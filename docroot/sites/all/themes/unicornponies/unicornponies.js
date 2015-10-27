// $Id$

function addOverlay() { 
  var overlay = document.createElement("div");
  overlay.id = "overlay";
  document.body.appendChild(overlay);
}

window.onload = addOverlay;

this.onclick = function() {
  cornify_add();
}
