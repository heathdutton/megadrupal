jQuery(document).ready(function($){
    $("#myCanvasContainer").parent('div').parent('.block').css('padding', '15px');
});

window.onload = function() {
  try {
    TagCanvas.Start('myCanvas','tags',{
      textColour: null,
      // '#ff0000',
      outlineColour : "#0088cc",
      outiliMethod  : "outline",
      reverse: true,
      depth: 0.75,
      maxSpeed : 0.01,
      minSpeed : 0.01,
      // textFont: "Helvetica, Arial, sans-serif",
      textFont: null,
      textHeight: 13,
      zoom: 1.2,
      shape: 'vcylinder',
      hideTags: true,
      interval 	: 20,
      dragControl	: true,
      initial 	: [0.8,-0.3],
      decel 		: 0.9,
      txtOpt		: true,
      txtScale   	: 2,
      outlineThickness: 0.8,
      outlineOffset	: 2,
      pulsateTo	: 0.8,
      pulsateTime	: 2,
      shadow 	: "#505050",
      shadowBlur 	: 5,
      shadowOffset	: [2,2],
      weight 	: true,
      weightMode 	: "size",
      weightSize 	: 0.8,
      weightFrom 	: 1,
      weightSizeMin: 1.2,
      weightSizeMax: 2.8,
      freezeActive	: true,
      freezeDecel	: true,
      textOpt : true,
      wheelZoom:false
    });
  } catch(e) {
    // something went wrong, hide the canvas container
    document.getElementById('myCanvasContainer').style.display = 'none';
  }
};
