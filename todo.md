Week 2:

- [x] Gegevens meten in arduino en verzenden naar gateway
- [x] De gateway gegevens opslaan in Database

Week 3:
- [x] Probleem herhalende requets dat alleen de helft slaagt.
- [x] Architectuurschets + globale projectdocumentatie: functionaliteit, wat doet die wanneer?
- [x] De gateway stuurt asyncrhoon de ontvangen gegevens naar de jorg api
- [x] Code refectoren als nodig
- [x] Lampjes laten branden bij temperaturen

Week 4:
- [x] Testframeworks, eventueel automatisch, bedenkenen en opzeten
- [x] DHCP implementeren op arduino en pi en documenteren handleiding
- [x] Als de arduino opstart en nog geen id heeft dan een request sturen naar de gateway met huidige ip en id `-1`. De response van de gateway bevat de nieuwe id en die wordt opgeslagen in eeprom.
- [x] Als de arduino opstart en al een id in eeprom heeft staan dan een request naar de gateway met huidige id en ip. De response van de gateway hoeft niet worden geparsed.

Week 5:
- [x] Arduino gebruiken als http server om settings te ontvangen.
- [x] Temperatuurgrenzen configurabel maken via gateway. Vanaf gateway requestje naar arduino en daar dingen opslaan.
- [x] Gateway deployment op Pi met docker

Week 6:
- [x] Gateway gegevens/grafiek opvragen
- [x] Temperatuurmeter eiken
- [x] Tijdsyncrhonisatie; Arduino vraagt aan gateway de huidge tijd op bij startup. De gateway vraagt dit weer op aan de jorgvisch-api. De arduino houdt dan lokaal de tijd bij om de tijd te gebruiken bij verzenden. Dus ook extra veld meesturen naar gateway met timestamp.
- [x] Voorpagina uitleg schrijven (waar is wat te vinden?)
- [x] Tutorial Arduino Setup schrijven (how to install arduino?)

extra's:
- [x] Knopje toevoegen voor ID resetten
- [x] Thresholds met knopjes in de gateway dashboard aanpassen
- [x] Birth: Interactieve serial uitleg voor opzetten child
- [ ] arduino fritzing updaten met ethernet shield (Bram)
- [ ] Spell check

ideeen voor later:
- DNS Server
- Data opnieuw versturen naar Jorg als dat niet lukt
- Authenticatie tussen Child en Gateway
-  acknowleging receivement of new id



