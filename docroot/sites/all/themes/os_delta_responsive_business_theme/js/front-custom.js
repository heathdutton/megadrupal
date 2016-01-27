$(document).ready(function () {

	if($('.imgGallery').length>0 || $('.featuresBox').length>0) {var controller = $.superscrollorama();}
	
		 var a2 = $('.imgGallery .rowImages a');
	for (var i = 0; i < a2.length; i++) {
	controller.addTween($(a2[i]),TweenMax.fromTo(
		$(a2[i]),.5,
		{css:{'opacity':'0','-webkit-transform':'scale(0)','-moz-transform':'scale(0)',
		'-ms-transform':'scale(0)','-o-transform':'scale(0)','transform':'scale(0)'},
		immediateRender:true,ease:Quad.easeInOut},{css:{'opacity':'1','-webkit-transform':'scale(1)',
		'-moz-transform':'scale(1)','-ms-transform':'scale(1)','-o-transform':'scale(1)','transform':'scale(1)'},
		ease:Quad.easeInOut}));
	}

		var a3 = $('.featuresBox ul li');
	for (var i = 0; i < a3.length; i++) {
	controller.addTween($(a3[i]),TweenMax.fromTo(
		$(a3[i]),.7,
		{css:{'opacity':'0','-webkit-transform':'scale(0)','-moz-transform':'scale(0)',
		'-ms-transform':'scale(0)','-o-transform':'scale(0)','transform':'scale(0)'},
		immediateRender:true,ease:Quad.easeInOut},{css:{'opacity':'1','-webkit-transform':'scale(1)',
		'-moz-transform':'scale(1)','-ms-transform':'scale(1)','-o-transform':'scale(1)','transform':'scale(1)'},
		ease:Quad.easeInOut}));
	}

 });