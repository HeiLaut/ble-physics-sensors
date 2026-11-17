If this sketch is loaded onto the light gate (**single version with optional OLED**),
the experiment must be added to phyphox manually, either via the provided QR code
or the .phyphox file, which can be found in the PhyPhox-Files folder.

If the shadowing-time readings are wrong,
digitalRead(SIGNAL_PIN) must be replaced with !digitalRead(SIGNAL_PIN)
in lines 76 and 152,

or vice versa.

This is because some boards output a LOW signal when the beam is broken, while others output a HIGH signal.


---------------------------------

Wird dieser Sketch auf die Lichtschranke (**einzelne Version mit optionalem OLED**) geladen,
muss das Experiment manuell zu phyphox hinzugefügt werden, entweder über den mitgelieferten QR-Code
oder die .phyphox-Datei, die sich im Ordner PhyPhox-Files befindet.

Sind die Messwerte der Verdunklungszeiten falsch,
ist in den Zeilen 76 und 152
digitalRead(SIGNAL_PIN) durch !digitalRead(SIGNAL_PIN) zu ersetzen

oder umgekehrt.

Der Grund: Manche Boards geben bei unterbrochenem Strahl ein LOW-Signal aus, andere ein HIGH-Signal.