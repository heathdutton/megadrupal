<?php
/**
 * @file
 * Inline embedding means that we include the output into a javascript file
 */
?>

if(typeof addOnloadEvent != 'function') {
  function addOnloadEvent(fnc){
    //alert('addOnloadEvent is adding ' + fnc);
    if ( typeof window.addEventListener != "undefined" ) {
      //alert(fnc + ' was added as window.addEventListener');
      window.addEventListener( "load", fnc, false );
    }
    else if ( typeof window.attachEvent != "undefined" ) {
      //alert(fnc + ' was added as window.attachEvent');
      window.attachEvent( "onload", fnc );
    }
    else {
      if ( window.onload != null ) {
        var oldOnload = window.onload;
        window.onload = function ( e ) {
          oldOnload( e );
          window[fnc]();
          //alert(fnc + ' was added to the existing window.onload');
        };
      }
      else
        window.onload = fnc;
        //alert(fnc + ' was added as a new window.onload');
    }
  }
}
addOnloadEvent(function(){
  document.getElementById(widgetContext_<?php print $id_suffix; ?>['widgetid']).innerHTML = <?php print $js_string ?>;
});
