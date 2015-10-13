var messageParent = function(scrollTop){
  var h = document.body.scrollHeight;
  h = (scrollTop)? h+'s':h;
  if(top.postMessage){
    top.postMessage( h , '*');
  } else {
    window.location.hash = 'h'+h;
  }
}
window.onload = function() {
  messageParent();
}
window.onresize = function() {
  messageParent();
}
 
if (window.addEventListener) {
  window.addEventListener("DOMSubtreeModified", function(){messageParent();}, true);
} else if (window.attachEvent) {
  window.attachEvent("onDOMSubtreeModified", function(){messageParent();});
}