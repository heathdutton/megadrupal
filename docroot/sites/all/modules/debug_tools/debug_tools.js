/**
 * @file
 * Debug Tools Module JS File.
 */

 window.onload = function(){
  debug_toolscreateCookies(7);
};

window.onresize = function(){
  debug_toolscreateCookies(7);
};

function debug_toolscreateCookies(days) {
  if (days) {
    var date = new Date();
    date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
    var expires = "; expires=" + date.toGMTString();
  }
  else {
    var expires = "";
    var w = Math.max(document.documentElement.clientWidth, window.innerWidth || 0)
    var h = Math.max(document.documentElement.clientHeight, window.innerHeight || 0)
    document.cookie = "debug_tools_vp_width=" + w + expires + "; path=/";
    document.cookie = "debug_tools_vp_height=" + h + expires + "; path=/";
  }
}
