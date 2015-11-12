// $Id: amelie.js,v 1.2.4.1 2010/06/06 05:10:06 cha0s Exp $

/**
 * @file
 * JavaScript file for the amelie module.
 */

if(document.all && !window.XMLHttpRequest){
  
var x = 1,when=0,str,dir,fil;
function amelie(){
  
  if(x) {
    str = Math.floor( Math.random() * 2 ) + 2;
    dir = Math.floor( Math.random() * 360 );
    setTimeout( 'amelie()', 500 );
  }
  else {
    str = 0; dir = 0;
    when = Math.floor( Math.random() * 10000 ) + 2000;
    setTimeout( 'amelie()', when );
  }
  
  var fil = "progid:DXImageTransform.Microsoft.MotionBlur(strength=" + 
            str + ",direction=" + dir + ",enabled='true')";
  document.body.style.filter = fil;
  x = x ^ 1;
}

setTimeout('amelie()',1000);
}
