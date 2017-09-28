Week 2:

- [x] Gegevens meten in arduino en verzenden naar gateway
- [x] De gateway gegevens opslaan in Database

Week 3:
- [x] Probleem herhalende requets dat alleen de helft slaagt.
- [ ] Architectuurschets + globale projectdocumentatie: functionaliteit, wat doet die wanneer?
- [x] De gateway stuurt asyncrhoon de ontvangen gegevens naar de jorg api
- [x] Code refectoren als nodig
- [x] Lampjes laten branden bij temperaturen

Week 4:
- [ ] Testframeworks, eventueel automatisch, bedenkenen en opzeten
- [x] DHCP implementeren op arduino en pi en documenteren handleiding
- [ ] Nieuwe weerstation's automatisch een nieuw id krijgen van gateway en id opslaan op de arduino. Wanneer de arduino opnieuw opstart dat hij niet meer vraagt om id en het id laad en daarmee data gaat verzenden.

Week 5:
- [ ] Arduino gebruiken als http server om settings te ontvangen.
- [ ] Temperatuurgrenzen configurabel maken via gateway. Vanaf gateway requestje naar arduino en daar dingen opslaan.
- [ ] Gateway deployment op Pi met docker

Week 6:
- [ ] Gateway gegevens/grafiek opvragen
- [ ] Temperatuurmeter eiken
- [ ] Tijdsyncrhonisatie; Arduino vraagt aan gateway de huidge tijd op bij startup. De gateway vraagt dit weer op aan de jorgvisch-api. De arduino houdt dan lokaal de tijd bij om de tijd te gebruiken bij verzenden. Dus ook extra veld meesturen naar gateway met timestamp.


* Knopje toevoegen voor ID resetten
* DNS Server
* Data opnieuw versturen naar Jorg als dat niet lukt
* Authenticatie tussen Child en Gateway
