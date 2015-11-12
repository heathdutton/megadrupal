FEN module
----------

FEN, the Forsyth-Edwards Notation is a syntax for defining a chessboard layout: 
http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

The FEN module provides the ability to add chessboard diagrams directly to Drupal pages 
using the following syntax:
<fen>FEN board definition</fen>

e.g.
<fen>rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR</fen>

Each text format which should be allowed to include the fen diagrams must be configured to 
enable the FEN chessboard filter. 

The text formats are available in your drupal website at:
q=admin/config/content/formats
 