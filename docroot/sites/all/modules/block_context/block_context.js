jQuery(document).ready(function() { 

  // context select dropdown auto change	
  jQuery('#block-context-select').change( function() {
    window.location.href = jQuery(this).val();
  });

  jQuery('select.block-context').selectList();

/*jQuery('#block-context-menu ul li a').expander({
    slicePoint:       10,  // default is 100
    expandText:         '...', // default is 'read more...'
    collapseTimer:    5000, // re-collapses after 5 seconds; default is 0, so no re-collapsing
    userCollapseText: '[^]'  // default is '[collapse expanded text]'
  });
*/

  jQuery('#block-context-menu ul li').each(function(){
	
	var width = jQuery(this).width();

	if(width > 140) {
	
	    jQuery(this).wrap('<span class="more-arrow">');
	  	jQuery(this).addClass('width ' + width);
	    //jQuery(this).addClass('ellipsis');
	    jQuery(this).css('width', 140);
	
	  //jQuery('a' , this).text(jQuery('a' , this) + '...');

	}
  });

  jQuery('#block-context-menu ul li').hover(function(){
		
	if(jQuery(this).width() == 140) {
		
	  var width = jQuery(this).attr("class").split(' ');
	  width = width[1];
	
      jQuery(this).animate(
	
	    { width: width + 'px' },
	    { duration: 2000 },
        function() {
	      // not yet working
          jQuery('.more-arrow', this).remove('.more-arrow');
        }
      );

    }
  });


//var width = alert(jQuery('.selectlist-select').css('width')); 
////alert(jQuery('.selectlist-select').css('width')); 
////alert(jQuery('.selectlist-select').attr('width')); 
//
//jQuery('.selectlist-list li').each( function() {
//  
//  //jQuery(this).width(width)); 
//  //alert(jQuery(this).attr('width')); 
//  jQuery(this).css('width', '100px'); 
//});

});