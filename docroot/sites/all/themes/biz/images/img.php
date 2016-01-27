<?
/*
To draw the PNG - call this script with a URL like the following:
*/
if(function_exists('imagettfbbox')) {
$font_dir = "fonts/";
$fonts = array();
if ( $dh = opendir ($font_dir) ) {
	while ( false !== ( $font_file = readdir ( $dh ) ) ) {
		if (strtolower(substr($font_file, -4)) == ".ttf") {
			$fonts [$font_file] = $font_dir.$font_file;

		}
	}
	closedir ( $dh );
}
include('textPNG.class.php');
Header("Content-type: image/png");
$text = new textPNG;
if (isset($_GET['msg']))$text->msg = $_GET['msg']; // text to display
if (isset($_GET['font'])){ if(isset($fonts[$_GET['font']])){ $text->font =  $fonts[$_GET['font']];}} // font to use (include directory if needed).
if (isset($_GET['size'])) $text->size =  $_GET['size']; // size in points
$text->draw();

}
?>
