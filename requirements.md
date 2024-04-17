# Anforderungen

## Antestat

- [x] HTTP Anfrage annehmen
- [x] HTML Antwort senden
- [x] HTML Antwort beinhaltet benötigte einträge
    - [x] HTTP-Methode
    - [x] Ressource
    - [x] Protokollversion
- [x] nicht-standardkonforme Methoden wie `MEIN_GET` wird akzeptiert => only `GET` is allowed
- [x] überlange Ressourcen werden akzeptiert => absolute path is limited to `4096` bytes

## Abtestat

- [x] Vorherig implementierte HTML Antwort nun nur unter `/debug` ereichbar
- [x] Datei aus document root wird gesendet
- [x] Auf Datein auserhalb des document root wird mit `403 Forbidden` geantwortet
- [x] Auf Datein die nicht existieren wird mit `404 Not Found` geantwortet
- [x] Auf Datein die nicht gelesen werden können wird mit `403 Forbidden` geantwortet
- [x] Ist die HTTP Methode nicht `GET` wird mit `501 Not Implemented` geantwortet
- [ ] Valgrind zeigt keine Speicherlecks
- [ ] Google Address Sanitizer zeigt keine Speicherlecks
