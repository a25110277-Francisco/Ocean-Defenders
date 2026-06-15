#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>
#include "core/EstadoJuego.hpp"
#include "core/Puntaje.hpp"
#include "core/Temporizador.hpp"
#include "entidades/Arrecife.hpp"
#include "entidades/BarreraCoral.hpp"
#include "entidades/Calamar.hpp"
#include "entidades/Cangrejo.hpp"
#include "entidades/Medusa.hpp"
#include "entidades/PezGlobo.hpp"
#include "entidades/Submarino.hpp"
#include "proyectiles/ProyectilEnemigo.hpp"
#include "proyectiles/Torpedo.hpp"
#include "recolectables/BurbujaOxigeno.hpp"
#include "recolectables/DobleDisparo.hpp"
#include "recolectables/Recolectable.hpp"
#include "recolectables/SuministroOxigeno.hpp"
#include "recolectables/VelocidadAumentada.hpp"
#include "rondas/AsaltoMasivo.hpp"
#include "rondas/InvasionInicial.hpp"
#include "rondas/OleadaReforzada.hpp"
#include "rondas/Ronda.hpp"
#include "rondas/RondaJefe.hpp"

class Juego {
public:
    Juego()
        : ventana(sf::VideoMode({AnchoVentana, AltoVentana}), "Ocean Defenders"),
          vistaJuego(sf::FloatRect({0.0f, 0.0f}, {static_cast<float>(AnchoVentana), static_cast<float>(AltoVentana)})),
          barreras{BarreraCoral(Posicion(110.0f, 508.0f)), BarreraCoral(Posicion(300.0f, 508.0f)),
                   BarreraCoral(Posicion(490.0f, 508.0f)), BarreraCoral(Posicion(680.0f, 508.0f))} {
        ventana.setFramerateLimit(60);
        ActualizarVista(ventana.getSize());
        CargarRecursos();
        CrearRondas();
    }

    void Iniciar() {
        estadoJuego.Asignar(EstadoJuego::Tipo::Jugando);
        mostrandoTitulo = true;
        puntaje.Reiniciar();
        temporizador.Reiniciar();
        submarino = Submarino();
        arrecife = Arrecife();
        rondaActual = 0;
        direccionFormacion = 1.0f;
        temporizadorBurbuja = 0.0f;
        temporizadorPowerUpJefe = 0.0f;
        tiempoAnimacionEnemigos = 0.0f;
        tiempoMuerteSubmarino = 0.0f;
        submarinoEnMuerte = false;
        musicaPrincipal.stop();
        torpedos.clear();
        proyectilesEnemigos.clear();
        recolectables.clear();
        efectosMuerte.clear();
        CrearRondaActual();
    }

    void Ejecutar() {
        Iniciar();
        sf::Clock reloj;

        while (ventana.isOpen()) {
            ProcesarEventos();

            const float delta = std::min(reloj.restart().asSeconds(), 0.05f);
            ActualizarMuerteSubmarino(delta);
            if (estadoJuego.EstaJugando() && !mostrandoTitulo) {
                Actualizar(delta);
            }

            Dibujar();
        }
    }

    void Actualizar(float delta) {
        temporizador.Actualizar(delta);
        tiempoAnimacionEnemigos += delta;
        ProcesarInput(delta);
        submarino.Actualizar(delta, static_cast<float>(AnchoVentana));
        ActualizarBurbuja(delta);
        ActualizarEnemigos(delta);
        ActualizarProyectiles(delta);
        ActualizarRecolectables(delta);
        ActualizarEfectosMuerte(delta);
        ResolverColisiones();
        LimpiarObjetos();
        VerificarDerrota();
        VerificarVictoria();
    }

    void Dibujar() {
        ventana.clear(sf::Color::Black);
        ventana.setView(vistaJuego);
        DibujarFondo();
        DibujarHud();

        arrecife.Dibujar(ventana, texturaArrecifeCargada ? &texturaArrecife : nullptr);
        for (std::size_t i = 0; i < cantidadBarrerasActivas; ++i) {
            barreras[i].Dibujar(ventana, texturaBarreraCargada ? &texturaBarrera : nullptr);
        }

        if (submarinoEnMuerte) {
            DibujarMuerteSubmarino();
        } else {
            submarino.Dibujar(ventana, texturasHeroeCargadas ? &texturaSubmarino : nullptr);
        }

        for (const auto& enemigo : enemigos) {
            enemigo->Dibujar(ventana, ObtenerTexturaMovimiento(*enemigo), tiempoAnimacionEnemigos);
        }
        DibujarEfectosMuerte();
        DibujarBarraVidaJefe();
        for (const auto& torpedo : torpedos) {
            torpedo.Dibujar(ventana, texturasHeroeCargadas ? &texturaTorpedo : nullptr);
        }
        for (const auto& proyectil : proyectilesEnemigos) {
            proyectil->Dibujar(ventana);
        }
        for (const auto& recolectable : recolectables) {
            recolectable->Dibujar(ventana, ObtenerTexturaRecolectable(*recolectable));
        }

        if (mostrandoTitulo) {
            DibujarPantallaTitulo();
        } else if (estadoJuego.Termino()) {
            DibujarPantallaFinal();
        }

        ventana.display();
    }

    void CambiarRonda() {
        if (rondaActual + 1 >= rondas.size()) {
            estadoJuego.Asignar(EstadoJuego::Tipo::Victoria);
            return;
        }

        ++rondaActual;
        submarino.RestaurarOxigeno();
        direccionFormacion = 1.0f;
        torpedos.clear();
        proyectilesEnemigos.clear();
        recolectables.clear();
        efectosMuerte.clear();
        temporizadorBurbuja = 0.0f;
        temporizadorPowerUpJefe = 0.0f;
        CrearRondaActual();
    }

    EstadoJuego VerificarVictoria() {
        if (estadoJuego.Termino()) {
            return estadoJuego;
        }

        if (rondas[rondaActual]->EsRondaJefe() && enemigos.empty() && efectosMuerte.empty()) {
            estadoJuego.Asignar(EstadoJuego::Tipo::Victoria);
            musicaPrincipal.stop();
        }
        return estadoJuego;
    }

    EstadoJuego VerificarDerrota() {
        if (estadoJuego.Termino()) {
            return estadoJuego;
        }

        if (submarino.ObtenerBarraOxigeno().EstaAgotada()) {
            IniciarMuerteSubmarino();
            estadoJuego.Asignar(EstadoJuego::Tipo::DerrotaOxigeno);
            musicaPrincipal.stop();
        } else if (arrecife.Destruido()) {
            estadoJuego.Asignar(EstadoJuego::Tipo::DerrotaArrecife);
            musicaPrincipal.stop();
        } else {
            for (const auto& enemigo : enemigos) {
                if (enemigo->LlegoAlFondo(LimiteInvasionY)) {
                    estadoJuego.Asignar(EstadoJuego::Tipo::DerrotaInvasion);
                    musicaPrincipal.stop();
                    break;
                }
            }
        }
        return estadoJuego;
    }

private:
    static constexpr unsigned int AnchoVentana = 900;
    static constexpr unsigned int AltoVentana = 700;
    static constexpr float LimiteInvasionY = 595.0f;
    static constexpr float DuracionMuerte = 0.8f;

    struct EfectoMuerte {
        std::string nombre;
        Posicion posicion;
        float tiempo{0.0f};
    };

    void CrearRondas() {
        rondas[0] = std::make_unique<InvasionInicial>();
        rondas[1] = std::make_unique<OleadaReforzada>();
        rondas[2] = std::make_unique<AsaltoMasivo>();
        rondas[3] = std::make_unique<RondaJefe>();
    }

    void CargarRecursos() {
        fuenteCargada = fuente.openFromFile("assets/fonts/PIXEL LETTER/Pixel Digivolve.otf");
        if (!fuenteCargada) {
            fuenteCargada = fuente.openFromFile("assets/fonts/NotoSans-Regular.ttf");
        }
        if (!fuenteCargada) {
            fuenteCargada = fuente.openFromFile("C:/Windows/Fonts/arial.ttf");
        }

        texturaBarreraCargada = texturaBarrera.loadFromFile("assets/stage/wall.png");
        texturaArrecifeCargada = texturaArrecife.loadFromFile("assets/stage/reef.png");
        if (texturaBarreraCargada) {
            texturaBarrera.setSmooth(false);
        }
        if (texturaArrecifeCargada) {
            texturaArrecife.setSmooth(false);
        }

        texturasEnemigosCargadas =
            texturaCangrejo.loadFromFile("assets/enemies/crab.PNG") &&
            texturaCangrejoMuerto.loadFromFile("assets/enemies/crabDead.png") &&
            texturaCalamar.loadFromFile("assets/enemies/squit.png") &&
            texturaCalamarMuerto.loadFromFile("assets/enemies/squitDead.png") &&
            texturaMedusa.loadFromFile("assets/enemies/jellyfish.PNG") &&
            texturaMedusaMuerta.loadFromFile("assets/enemies/jellyfishDead.PNG") &&
            texturaPezGlobo.loadFromFile("assets/enemies/pufferFish.PNG") &&
            texturaPezGloboMuerto.loadFromFile("assets/enemies/pufferFishDead.png");
        if (texturasEnemigosCargadas) {
            texturaCangrejo.setSmooth(false);
            texturaCangrejoMuerto.setSmooth(false);
            texturaCalamar.setSmooth(false);
            texturaCalamarMuerto.setSmooth(false);
            texturaMedusa.setSmooth(false);
            texturaMedusaMuerta.setSmooth(false);
            texturaPezGlobo.setSmooth(false);
            texturaPezGloboMuerto.setSmooth(false);
        }

        texturasHeroeCargadas =
            texturaSubmarino.loadFromFile("assets/hero/submarine.PNG") &&
            texturaSubmarinoMuerto.loadFromFile("assets/hero/submarineDead.png") &&
            texturaTorpedo.loadFromFile("assets/hero/torpedo.png");
        if (texturasHeroeCargadas) {
            texturaSubmarino.setSmooth(false);
            texturaSubmarinoMuerto.setSmooth(false);
            texturaTorpedo.setSmooth(false);
        }

        texturasPowerUpsCargadas =
            texturaDobleDisparo.loadFromFile("assets/powerUps/double.png") &&
            texturaVelocidad.loadFromFile("assets/powerUps/fast.png") &&
            texturaOxigeno.loadFromFile("assets/powerUps/oxigeno.png");
        if (texturasPowerUpsCargadas) {
            texturaDobleDisparo.setSmooth(false);
            texturaVelocidad.setSmooth(false);
            texturaOxigeno.setSmooth(false);
        }

        musicaPrincipalCargada = musicaPrincipal.openFromFile("assets/music/principal.ogg");
        if (musicaPrincipalCargada) {
            musicaPrincipal.setLooping(true);
            musicaPrincipal.setVolume(35.0f);
        }
    }

    void ActualizarVista(sf::Vector2u tamanoVentana) {
        const float ancho = static_cast<float>(tamanoVentana.x);
        const float alto = static_cast<float>(tamanoVentana.y);
        const float proporcionVentana = ancho / alto;
        const float proporcionJuego = static_cast<float>(AnchoVentana) / static_cast<float>(AltoVentana);

        float viewportX = 0.0f;
        float viewportY = 0.0f;
        float viewportAncho = 1.0f;
        float viewportAlto = 1.0f;

        if (proporcionVentana > proporcionJuego) {
            viewportAncho = proporcionJuego / proporcionVentana;
            viewportX = (1.0f - viewportAncho) * 0.5f;
        } else {
            viewportAlto = proporcionVentana / proporcionJuego;
            viewportY = (1.0f - viewportAlto) * 0.5f;
        }

        vistaJuego.setViewport(sf::FloatRect({viewportX, viewportY}, {viewportAncho, viewportAlto}));
        ventana.setView(vistaJuego);
    }

    void CrearRondaActual() {
        enemigos.clear();
        const Ronda& ronda = *rondas[rondaActual];
        RegenerarBarreras();

        if (ronda.EsRondaJefe()) {
            enemigos.push_back(std::make_unique<PezGlobo>(Posicion(380.0f, 120.0f)));
            submarino.RestaurarOxigeno();
            return;
        }

        int fila = 0;
        AgregarGrupoCangrejos(ronda.ObtenerCangrejos(), fila);
        fila += FilasParaCantidad(ronda.ObtenerCangrejos());
        AgregarGrupoCalamares(ronda.ObtenerCalamares(), fila);
        fila += FilasParaCantidad(ronda.ObtenerCalamares());
        AgregarGrupoMedusas(ronda.ObtenerMedusas(), fila);
    }

    void RegenerarBarreras() {
        cantidadBarrerasActivas = rondaActual == 0 ? 4 : (rondaActual == 1 ? 3 : 2);

        for (auto& barrera : barreras) {
            barrera = BarreraCoral(Posicion(-200.0f, -200.0f));
        }

        const float separacion = static_cast<float>(AnchoVentana) / static_cast<float>(cantidadBarrerasActivas + 1);
        for (std::size_t i = 0; i < cantidadBarrerasActivas; ++i) {
            const float posicionX = separacion * static_cast<float>(i + 1) - 60.0f;
            barreras[i] = BarreraCoral(Posicion(posicionX, 498.0f));
        }
    }

    int FilasParaCantidad(int cantidad) const {
        if (cantidad <= 0) {
            return 0;
        }
        return static_cast<int>(std::ceil(static_cast<float>(cantidad) / 7.0f));
    }

    Posicion PosicionEnFormacion(int indice, int filaInicial) const {
        const int columna = indice % 7;
        const int fila = filaInicial + indice / 7;
        return {72.0f + static_cast<float>(columna) * 115.0f, 94.0f + static_cast<float>(fila) * 84.0f};
    }

    void AgregarGrupoMedusas(int cantidad, int filaInicial) {
        for (int i = 0; i < cantidad; ++i) {
            enemigos.push_back(std::make_unique<Medusa>(PosicionEnFormacion(i, filaInicial)));
        }
    }

    void AgregarGrupoCalamares(int cantidad, int filaInicial) {
        for (int i = 0; i < cantidad; ++i) {
            enemigos.push_back(std::make_unique<Calamar>(PosicionEnFormacion(i, filaInicial)));
        }
    }

    void AgregarGrupoCangrejos(int cantidad, int filaInicial) {
        for (int i = 0; i < cantidad; ++i) {
            enemigos.push_back(std::make_unique<Cangrejo>(PosicionEnFormacion(i, filaInicial)));
        }
    }

    void ProcesarEventos() {
        while (const std::optional evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) {
                ventana.close();
            }

            if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
                if (tecla->code == sf::Keyboard::Key::Escape) {
                    ventana.close();
                }
                if (mostrandoTitulo && tecla->code == sf::Keyboard::Key::Enter) {
                    mostrandoTitulo = false;
                    if (musicaPrincipalCargada) {
                        musicaPrincipal.play();
                    }
                } else if (estadoJuego.Termino() && tecla->code == sf::Keyboard::Key::Enter) {
                    Iniciar();
                }
            }

            if (const auto* redimension = evento->getIf<sf::Event::Resized>()) {
                ActualizarVista(redimension->size);
            }
        }
    }

    void ProcesarInput(float delta) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            submarino.MoverIzquierda(delta);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            submarino.MoverDerecha(delta);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            auto nuevosTorpedos = submarino.DispararTorpedos();
            for (auto& torpedo : nuevosTorpedos) {
                torpedos.push_back(torpedo);
            }
        }
    }

    void ActualizarBurbuja(float delta) {
        temporizadorBurbuja += delta;
        if (temporizadorBurbuja >= 15.0f) {
            temporizadorBurbuja = 0.0f;
            recolectables.push_back(std::make_unique<BurbujaOxigeno>(Posicion(NumeroAleatorio(35.0f, 840.0f), 84.0f)));
        }
    }

    void ActualizarEnemigos(float delta) {
        if (rondas[rondaActual]->EsRondaJefe()) {
            ActualizarJefe(delta);
            return;
        }

        const float multiplicador = rondas[rondaActual]->ObtenerMultiplicadorVelocidad();
        bool debeDescender = false;

        for (auto& enemigo : enemigos) {
            enemigo->MoverFormacion(direccionFormacion, delta, multiplicador);
            if (enemigo->TocaBorde(static_cast<float>(AnchoVentana))) {
                debeDescender = true;
            }

            if (enemigo->PuedeDisparar() && NumeroAleatorio(0.0f, 1.0f) < 0.003f) {
                proyectilesEnemigos.push_back(enemigo->Disparar());
            }
        }

        if (debeDescender) {
            direccionFormacion *= -1.0f;
            for (auto& enemigo : enemigos) {
                enemigo->DescenderFila();
            }
        }
    }

    void ActualizarJefe(float delta) {
        if (enemigos.empty()) {
            return;
        }

        auto* jefe = dynamic_cast<PezGlobo*>(enemigos.front().get());
        if (!jefe) {
            return;
        }

        jefe->MoverErratico(delta, static_cast<float>(AnchoVentana));
        if (jefe->PuedeDisparar()) {
            auto rafaga = jefe->DispararRafagaTriple();
            for (auto& proyectil : rafaga) {
                proyectilesEnemigos.push_back(std::move(proyectil));
            }
        }

        temporizadorPowerUpJefe += delta;
        if (temporizadorPowerUpJefe >= 30.0f) {
            temporizadorPowerUpJefe = 0.0f;
            CrearPowerUpAleatorio(jefe->ObtenerPosicionDisparo());
        }
    }

    void ActualizarProyectiles(float delta) {
        for (auto& torpedo : torpedos) {
            torpedo.Actualizar(delta);
            if (torpedo.ObtenerPosicion().y < -30.0f) {
                torpedo.Desactivar();
            }
        }

        for (auto& proyectil : proyectilesEnemigos) {
            proyectil->Actualizar(delta);
            const Posicion& posicion = proyectil->ObtenerPosicion();
            if (posicion.y > static_cast<float>(AltoVentana) + 30.0f || posicion.x < -50.0f || posicion.x > static_cast<float>(AnchoVentana) + 50.0f) {
                proyectil->Desactivar();
            }
        }
    }

    void ActualizarRecolectables(float delta) {
        for (auto& recolectable : recolectables) {
            recolectable->Actualizar(delta);
            if (recolectable->ObtenerPosicion().y > static_cast<float>(AltoVentana)) {
                recolectable->Desactivar();
            }
        }
    }

    void ActualizarEfectosMuerte(float delta) {
        for (auto& efecto : efectosMuerte) {
            efecto.tiempo += delta;
        }
        efectosMuerte.erase(std::remove_if(efectosMuerte.begin(), efectosMuerte.end(), [](const EfectoMuerte& efecto) {
            return efecto.tiempo >= DuracionMuerte;
        }), efectosMuerte.end());
    }

    void ResolverColisiones() {
        ResolverTorpedosContraEnemigos();
        ResolverProyectilesContraBarreras();
        ResolverProyectilesContraSubmarinoYArrecife();
        ResolverRecolectables();
    }

    void ResolverTorpedosContraEnemigos() {
        for (auto& torpedo : torpedos) {
            if (!torpedo.EstaActivo()) {
                continue;
            }

            for (auto& enemigo : enemigos) {
                if (!enemigo->Destruido() && HayColision(torpedo.ObtenerBounds(), enemigo->ObtenerBounds())) {
                    enemigo->RecibirDano(torpedo);
                    torpedo.Impactar();

                    if (enemigo->Destruido()) {
                        puntaje.Agregar(enemigo->ObtenerPuntaje());
                        if (ObtenerTexturaMuerte(*enemigo) != nullptr) {
                            efectosMuerte.push_back({enemigo->ObtenerNombre(), enemigo->ObtenerPosicion(), 0.0f});
                        }
                        if (!rondas[rondaActual]->EsRondaJefe() && NumeroAleatorio(0.0f, 1.0f) <= enemigo->ObtenerProbabilidadPowerUp()) {
                            CrearPowerUpAleatorio(enemigo->ObtenerPosicion());
                        }
                    }
                    break;
                }
            }
        }
    }

    void ResolverProyectilesContraBarreras() {
        for (auto& proyectil : proyectilesEnemigos) {
            if (!proyectil->EstaActivo()) {
                continue;
            }
            for (std::size_t i = 0; i < cantidadBarrerasActivas; ++i) {
                if (!barreras[i].Destruida() && HayColision(proyectil->ObtenerBounds(), barreras[i].ObtenerBounds())) {
                    barreras[i].RecibirImpacto(5);
                    proyectil->Impactar();
                    break;
                }
            }
        }
    }

    void ResolverProyectilesContraSubmarinoYArrecife() {
        for (auto& proyectil : proyectilesEnemigos) {
            if (!proyectil->EstaActivo()) {
                continue;
            }

            if (HayColision(proyectil->ObtenerBounds(), submarino.ObtenerBounds())) {
                submarino.RecibirDano(DanoOxigeno(proyectil->ObtenerDanoOxigeno()));
                proyectil->Impactar();
                continue;
            }

            if (HayColision(proyectil->ObtenerBounds(), arrecife.ObtenerBounds())) {
                arrecife.RecibirDano(proyectil->ObtenerDanoVida());
                proyectil->Impactar();
            }
        }
    }

    void ResolverRecolectables() {
        for (auto& recolectable : recolectables) {
            if (recolectable->EstaActivo() && HayColision(recolectable->ObtenerBounds(), submarino.ObtenerBounds())) {
                recolectable->Recolectar(submarino);
            }
        }
    }

    void LimpiarObjetos() {
        torpedos.erase(std::remove_if(torpedos.begin(), torpedos.end(), [](const Torpedo& torpedo) {
            return !torpedo.EstaActivo();
        }), torpedos.end());

        proyectilesEnemigos.erase(std::remove_if(proyectilesEnemigos.begin(), proyectilesEnemigos.end(), [](const std::unique_ptr<ProyectilEnemigo>& proyectil) {
            return !proyectil->EstaActivo();
        }), proyectilesEnemigos.end());

        recolectables.erase(std::remove_if(recolectables.begin(), recolectables.end(), [](const std::unique_ptr<Recolectable>& recolectable) {
            return !recolectable->EstaActivo();
        }), recolectables.end());

        enemigos.erase(std::remove_if(enemigos.begin(), enemigos.end(), [](const std::unique_ptr<Enemigo>& enemigo) {
            return enemigo->Destruido();
        }), enemigos.end());

        if (estadoJuego.EstaJugando() && enemigos.empty() && efectosMuerte.empty() && !rondas[rondaActual]->EsRondaJefe()) {
            CambiarRonda();
        }
    }

    const sf::Texture* ObtenerTexturaMovimiento(const Enemigo& enemigo) const {
        if (!texturasEnemigosCargadas) {
            return nullptr;
        }
        if (enemigo.ObtenerNombre() == "Cangrejo") {
            return &texturaCangrejo;
        }
        if (enemigo.ObtenerNombre() == "Calamar") {
            return &texturaCalamar;
        }
        if (enemigo.ObtenerNombre() == "Medusa") {
            return &texturaMedusa;
        }
        if (enemigo.ObtenerNombre() == "PezGlobo") {
            return &texturaPezGlobo;
        }
        return nullptr;
    }

    const sf::Texture* ObtenerTexturaMuerte(const Enemigo& enemigo) const {
        return ObtenerTexturaMuerte(enemigo.ObtenerNombre());
    }

    const sf::Texture* ObtenerTexturaMuerte(const std::string& nombre) const {
        if (!texturasEnemigosCargadas) {
            return nullptr;
        }
        if (nombre == "Cangrejo") {
            return &texturaCangrejoMuerto;
        }
        if (nombre == "Calamar") {
            return &texturaCalamarMuerto;
        }
        if (nombre == "Medusa") {
            return &texturaMedusaMuerta;
        }
        if (nombre == "PezGlobo") {
            return &texturaPezGloboMuerto;
        }
        return nullptr;
    }

    void DibujarEfectosMuerte() {
        constexpr int CantidadCuadros = 4;
        for (const auto& efecto : efectosMuerte) {
            const sf::Texture* textura = ObtenerTexturaMuerte(efecto.nombre);
            if (textura == nullptr) {
                continue;
            }

            const int cuadro = std::min(
                static_cast<int>(efecto.tiempo / (DuracionMuerte / CantidadCuadros)),
                CantidadCuadros - 1
            );
            const sf::Vector2u texturaTamano = textura->getSize();
            const int inicioX = cuadro * static_cast<int>(texturaTamano.x) / CantidadCuadros;
            const int finX = (cuadro + 1) * static_cast<int>(texturaTamano.x) / CantidadCuadros;
            const sf::IntRect recorte(
                {inicioX, 0},
                {finX - inicioX, static_cast<int>(texturaTamano.y)}
            );
            sf::Sprite sprite(*textura, recorte);
            const bool esJefe = efecto.nombre == "PezGlobo";
            const sf::Vector2f tamanoColision = esJefe ? sf::Vector2f{140.0f, 88.0f} : sf::Vector2f{65.625f, 46.875f};
            const sf::Vector2f tamanoVisual = esJefe ? sf::Vector2f{140.0f, 88.0f} : sf::Vector2f{90.625f, 81.25f};
            sprite.setPosition({
                efecto.posicion.x + (tamanoColision.x - tamanoVisual.x) * 0.5f,
                efecto.posicion.y + (tamanoColision.y - tamanoVisual.y) * 0.5f
            });
            sprite.setScale({
                tamanoVisual.x / static_cast<float>(recorte.size.x),
                tamanoVisual.y / static_cast<float>(recorte.size.y)
            });
            ventana.draw(sprite);
        }
    }

    void IniciarMuerteSubmarino() {
        if (!submarinoEnMuerte) {
            submarinoEnMuerte = true;
            tiempoMuerteSubmarino = 0.0f;
        }
    }

    void ActualizarMuerteSubmarino(float delta) {
        if (submarinoEnMuerte) {
            tiempoMuerteSubmarino = std::min(tiempoMuerteSubmarino + delta, DuracionMuerteSubmarino);
        }
    }

    void DibujarMuerteSubmarino() {
        if (!texturasHeroeCargadas) {
            submarino.Dibujar(ventana);
            return;
        }

        constexpr int CantidadCuadros = 6;
        const int cuadro = std::min(
            static_cast<int>(tiempoMuerteSubmarino / (DuracionMuerteSubmarino / CantidadCuadros)),
            CantidadCuadros - 1
        );
        const sf::Vector2u texturaTamano = texturaSubmarinoMuerto.getSize();
        const int inicioX = cuadro * static_cast<int>(texturaTamano.x) / CantidadCuadros;
        const int finX = (cuadro + 1) * static_cast<int>(texturaTamano.x) / CantidadCuadros;
        const sf::IntRect recorte(
            {inicioX, 0},
            {finX - inicioX, static_cast<int>(texturaTamano.y)}
        );
        const Posicion& posicion = submarino.ObtenerPosicion();
        const sf::Vector2f tamanoVisual{66.0f, 80.0f};
        sf::Sprite sprite(texturaSubmarinoMuerto, recorte);
        sprite.setPosition({
            posicion.x + (54.0f - tamanoVisual.x) * 0.5f,
            posicion.y + 30.0f - tamanoVisual.y
        });
        sprite.setScale({
            tamanoVisual.x / static_cast<float>(recorte.size.x),
            tamanoVisual.y / static_cast<float>(recorte.size.y)
        });
        ventana.draw(sprite);
    }

    const sf::Texture* ObtenerTexturaRecolectable(const Recolectable& recolectable) const {
        if (!texturasPowerUpsCargadas) {
            return nullptr;
        }

        const std::string nombre = recolectable.ObtenerNombre();
        if (nombre == "Doble disparo") {
            return &texturaDobleDisparo;
        }
        if (nombre == "Velocidad aumentada") {
            return &texturaVelocidad;
        }
        if (nombre == "Suministro oxigeno" || nombre == "Burbuja oxigeno") {
            return &texturaOxigeno;
        }
        return nullptr;
    }

    void CrearPowerUpAleatorio(Posicion posicion) {
        const int tipo = EnteroAleatorio(0, 2);
        if (tipo == 0) {
            recolectables.push_back(std::make_unique<DobleDisparo>(posicion));
        } else if (tipo == 1) {
            recolectables.push_back(std::make_unique<VelocidadAumentada>(posicion));
        } else {
            recolectables.push_back(std::make_unique<SuministroOxigeno>(posicion));
        }
    }

    bool HayColision(const sf::FloatRect& a, const sf::FloatRect& b) const {
        return a.findIntersection(b).has_value();
    }

    float NumeroAleatorio(float minimo, float maximo) {
        std::uniform_real_distribution<float> distribucion(minimo, maximo);
        return distribucion(generador);
    }

    int EnteroAleatorio(int minimo, int maximo) {
        std::uniform_int_distribution<int> distribucion(minimo, maximo);
        return distribucion(generador);
    }

    void DibujarFondo() {
        sf::RectangleShape zonaJuego({static_cast<float>(AnchoVentana), static_cast<float>(AltoVentana)});
        zonaJuego.setPosition({0.0f, 0.0f});
        zonaJuego.setFillColor(sf::Color(7, 38, 78));
        ventana.draw(zonaJuego);

        sf::RectangleShape hud({static_cast<float>(AnchoVentana), 76.0f});
        hud.setPosition({0.0f, 0.0f});
        hud.setFillColor(sf::Color(10, 24, 42));
        ventana.draw(hud);

        sf::RectangleShape fondo({static_cast<float>(AnchoVentana), 28.0f});
        fondo.setPosition({0.0f, 672.0f});
        fondo.setFillColor(sf::Color(32, 80, 72));
        ventana.draw(fondo);

        for (int i = 0; i < 12; ++i) {
            sf::CircleShape burbuja(2.0f + static_cast<float>(i % 4));
            burbuja.setPosition({40.0f + static_cast<float>(i) * 76.0f, 90.0f + std::fmod(temporizador.ObtenerSegundos() * (12.0f + i), 470.0f)});
            burbuja.setFillColor(sf::Color(80, 160, 190, 80));
            ventana.draw(burbuja);
        }

        DibujarBanderaLimite();
    }

    void DibujarHud() {
        DibujarBarra({24.0f, 18.0f}, {190.0f, 16.0f}, submarino.ObtenerBarraOxigeno().ObtenerPorcentaje(), sf::Color(80, 190, 255));
        DibujarBarra({24.0f, 48.0f}, {190.0f, 16.0f}, arrecife.ObtenerVida().ObtenerPorcentaje(), sf::Color(80, 220, 150));

        DibujarTexto("Oxigeno " + std::to_string(submarino.ObtenerBarraOxigeno().ObtenerActual()) + "/100", {225.0f, 11.0f}, 16);
        DibujarTexto("Arrecife " + std::to_string(arrecife.ObtenerVida().ObtenerActual()) + "/1300", {225.0f, 41.0f}, 16);
        DibujarTexto("Puntaje " + std::to_string(puntaje.ObtenerValor()), {475.0f, 18.0f}, 18);
        DibujarTexto("Ronda " + std::to_string(rondas[rondaActual]->ObtenerNumero()) + ": " + rondas[rondaActual]->ObtenerNombre(), {475.0f, 44.0f}, 16);
    }

    void DibujarBarra(sf::Vector2f posicion, sf::Vector2f tamano, float porcentaje, sf::Color color) {
        sf::RectangleShape fondo(tamano);
        fondo.setPosition(posicion);
        fondo.setFillColor(sf::Color(20, 50, 70));
        ventana.draw(fondo);

        sf::RectangleShape relleno({tamano.x * std::clamp(porcentaje, 0.0f, 1.0f), tamano.y});
        relleno.setPosition(posicion);
        relleno.setFillColor(color);
        ventana.draw(relleno);
    }

    void DibujarBarraVidaJefe() {
        if (!rondas[rondaActual]->EsRondaJefe() || enemigos.empty()) {
            return;
        }

        const auto* jefe = dynamic_cast<const PezGlobo*>(enemigos.front().get());
        if (!jefe || jefe->Destruido()) {
            return;
        }

        const sf::FloatRect bounds = jefe->ObtenerBounds();
        const sf::Vector2f posicionBarra(bounds.position.x, bounds.position.y - 18.0f);
        const sf::Vector2f tamanoBarra(bounds.size.x, 9.0f);

        DibujarBarra(posicionBarra, tamanoBarra, jefe->ObtenerVida().ObtenerPorcentaje(), sf::Color(220, 60, 210));
    }

    void DibujarBanderaLimite() {
        sf::RectangleShape linea({static_cast<float>(AnchoVentana), 3.0f});
        linea.setPosition({0.0f, LimiteInvasionY});
        linea.setFillColor(sf::Color(255, 70, 70));
        ventana.draw(linea);

        sf::RectangleShape mastil({5.0f, 38.0f});
        mastil.setPosition({18.0f, LimiteInvasionY - 36.0f});
        mastil.setFillColor(sf::Color(245, 245, 245));
        ventana.draw(mastil);

        sf::ConvexShape bandera;
        bandera.setPointCount(3);
        bandera.setPoint(0, {23.0f, LimiteInvasionY - 36.0f});
        bandera.setPoint(1, {78.0f, LimiteInvasionY - 22.0f});
        bandera.setPoint(2, {23.0f, LimiteInvasionY - 8.0f});
        bandera.setFillColor(sf::Color(255, 70, 70));
        ventana.draw(bandera);

        DibujarTexto("LIMITE", {86.0f, LimiteInvasionY - 32.0f}, 16, sf::Color(255, 235, 235));
    }

    void DibujarTexto(const std::string& texto, sf::Vector2f posicion, unsigned int tamano, sf::Color color = sf::Color::White) {
        if (!fuenteCargada) {
            return;
        }

        sf::Text etiqueta(fuente, texto, tamano);
        etiqueta.setFillColor(color);
        etiqueta.setPosition(posicion);
        ventana.draw(etiqueta);
    }

    void DibujarTextoCentrado(const std::string& texto, float centroX, float y, unsigned int tamano, sf::Color color = sf::Color::White) {
        if (!fuenteCargada) {
            return;
        }

        sf::Text etiqueta(fuente, texto, tamano);
        etiqueta.setFillColor(color);
        const sf::FloatRect bounds = etiqueta.getLocalBounds();
        etiqueta.setPosition({centroX - bounds.size.x * 0.5f - bounds.position.x, y});
        ventana.draw(etiqueta);
    }

    void DibujarPantallaFinal() {
        sf::RectangleShape capa({static_cast<float>(AnchoVentana), static_cast<float>(AltoVentana)});
        capa.setPosition({0.0f, 0.0f});
        capa.setFillColor(sf::Color(0, 0, 0, 150));
        ventana.draw(capa);

        DibujarTexto(estadoJuego.ObtenerMensaje(), {210.0f, 300.0f}, 28, sf::Color(245, 250, 255));
        DibujarTexto("ENTER PARA REGRESAR A LA PANTALLA DEL TITULO", {175.0f, 345.0f}, 20, sf::Color(180, 230, 255));
    }

    void DibujarPantallaTitulo() {
        sf::RectangleShape capa({static_cast<float>(AnchoVentana), static_cast<float>(AltoVentana)});
        capa.setPosition({0.0f, 0.0f});
        capa.setFillColor(sf::Color(0, 0, 0, 145));
        ventana.draw(capa);

        DibujarTextoCentrado("OCEAN DEFENDERS", static_cast<float>(AnchoVentana) * 0.5f, 260.0f, 42, sf::Color(130, 235, 255));
        DibujarTextoCentrado("PROTEGE EL ARRECIFE", static_cast<float>(AnchoVentana) * 0.5f, 320.0f, 20, sf::Color(220, 250, 255));
        DibujarTextoCentrado("PRESIONA ENTER PARA INICIAR", static_cast<float>(AnchoVentana) * 0.5f, 380.0f, 22, sf::Color(180, 230, 255));
    }

    sf::RenderWindow ventana;
    sf::View vistaJuego;
    sf::Font fuente;
    sf::Texture texturaBarrera;
    sf::Texture texturaArrecife;
    sf::Texture texturaCangrejo;
    sf::Texture texturaCangrejoMuerto;
    sf::Texture texturaCalamar;
    sf::Texture texturaCalamarMuerto;
    sf::Texture texturaMedusa;
    sf::Texture texturaMedusaMuerta;
    sf::Texture texturaPezGlobo;
    sf::Texture texturaPezGloboMuerto;
    sf::Texture texturaSubmarino;
    sf::Texture texturaSubmarinoMuerto;
    sf::Texture texturaTorpedo;
    sf::Texture texturaDobleDisparo;
    sf::Texture texturaVelocidad;
    sf::Texture texturaOxigeno;
    sf::Music musicaPrincipal;
    bool fuenteCargada{false};
    bool texturaBarreraCargada{false};
    bool texturaArrecifeCargada{false};
    bool texturasEnemigosCargadas{false};
    bool texturasHeroeCargadas{false};
    bool texturasPowerUpsCargadas{false};
    bool musicaPrincipalCargada{false};

    Submarino submarino;
    Arrecife arrecife;
    std::array<BarreraCoral, 4> barreras;
    std::array<std::unique_ptr<Ronda>, 4> rondas;
    Puntaje puntaje;
    EstadoJuego estadoJuego;
    Temporizador temporizador;

    std::vector<Torpedo> torpedos;
    std::vector<std::unique_ptr<ProyectilEnemigo>> proyectilesEnemigos;
    std::vector<std::unique_ptr<Recolectable>> recolectables;
    std::vector<std::unique_ptr<Enemigo>> enemigos;
    std::vector<EfectoMuerte> efectosMuerte;

    std::mt19937 generador{std::random_device{}()};
    std::size_t rondaActual{0};
    std::size_t cantidadBarrerasActivas{4};
    bool mostrandoTitulo{true};
    bool submarinoEnMuerte{false};
    float direccionFormacion{1.0f};
    float temporizadorBurbuja{0.0f};
    float temporizadorPowerUpJefe{0.0f};
    float tiempoAnimacionEnemigos{0.0f};
    float tiempoMuerteSubmarino{0.0f};
    static constexpr float DuracionMuerteSubmarino = 1.2f;
};
