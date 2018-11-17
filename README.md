# AlgGeo1_WS18
<dl>
<dt><b>Semester</b></dt>
<dd>WS 2018/19, Universität Heidelberg</dd>
<dt><b>Veranstaltung</b></dt>
<dd>Algebraische Geometrie I</dd>
<dt><b>Veranstaltungskürzel</b></dt>
<dd>MM11</dd>
<dt><b>Dozent</b></dt>
<dd>Prof. Dr. Venjakob</dd>
<dt><b>Weitere Links</b></dt>
<dd>https://www.mathi.uni-heidelberg.de/~otmar/venjakob.html#lehre</dd>
</dl>

Jegliche Inhalte sind **ohne Gewähr** und bieten keinen Ersatz für die
eigentliche Vorlesung oder die angegebene Literatur.

## Hilfe gesucht!

Bei den folgenden Punkten könnte der Übersetzer Hilfe gebrauchen:

* Nachlesen des Stoffs! Der Großanteil wird *während der Vorlesung*
  gesetzt, also passiert es leicht, dass sich Fehler einschleichen.
* Versehen von Lemmata, Definitionen, usw. mit schlagtreffenden
  Labels.  (`\label`) Diese werden in
  [Gerby](https://gerby-project.github.io/) verwendet.
* Ergänzen von fehlenden Grafiken und kommutativen Diagrammen.
  Diagramme können mit `\xymatrix` (Paket `xypic`) gesetzt werden.
* Semantik für die verwendeten *tags*, insbesondere für `\subsection`,
  `\subsubsection` und `\part`. (siehe `scripts/create-tags.cc`)
  
## Richtlinien

Das Skript wird aus Zeitgründen halbautomatisch mit
[LyX](https://www.lyx.org/) gesetzt. Die verschiedenen Kapitel (in
`.tex` Format) können aber mit einem üblichen LaTeX-Editor verändert
werden.

Zum Gebrauch mit Gerby/plasTeX sind einige Einschränkungen einzuhalten:

* Es soll immer mindestens ein Leerzeichen zwischen Math-Blöcken
  vorhanden sein. `$foo$ $bar` is also erlaubt, aber nicht
  `$foo$$bar$`.
* Zur Kapitelbezeichnung darf `\part*` nicht verwendet werden. Ersatz:
  `\chapter*`
* Alle Dateien sollen utf8-kodiert sein.
* Nur eine bestimmte Anzahl an LaTeX-Pakten ist kompatibel zu
  plasTeX. Insbesondere können `color`, `tikz-cd`, `enumitem`,
  `cancel`, `stackrel` und `mathtools` nicht verwendet werden.
