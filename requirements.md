# Anforderungen

## Antestat

- [x] HTTP Anfrage annehmen
- [x] HTML Antwort senden
- [ ] HTML Antwort beinhaltet benötigte einträge
    - [ ] HTTP-Methode
    - [ ] Ressource
    - [ ] Protokollversion
- [x] nicht-standardkonforme Methoden wie `MEIN_GET` wird akzeptiert
- [ ] überlange Ressourcen werden akzeptiert

## Abtestat

- [ ] Vorherig implementierte HTML Antwort nun nur unter `/debug` ereichbar
- [ ] Datei aus document root wird gesendet
- [ ] Auf Datein auserhalb des document root wird mit `403 Forbidden` geantwortet
- [x] Auf Datein die nicht existieren wird mit `404 Not Found` geantwortet
- [ ] Auf Datein die nicht gelesen werden können wird mit `403 Forbidden` geantwortet
- [x] Ist die HTTP Methode nicht `GET` wird mit `501 Not Implemented` geantwortet
- [ ] Valgrind zeigt keine Speicherlecks
- [ ] Google Address Sanitizer zeigt keine Speicherlecks
