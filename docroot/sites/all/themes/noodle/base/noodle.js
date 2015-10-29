// equal column heights (http://www.cssnewbie.com/equal-height-columns-with-jquery/)
function equalHeight(group) {
	var tallest = 0;
	group.each(function() {
		var thisHeight = jQuery(this).height();
		if(thisHeight > tallest) {
			tallest = thisHeight;
		}
	});
	group.height(tallest);
};
