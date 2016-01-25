jQuery(document).ready(function($) {
 $('a.twitter').mouseover(
     function() {
      $(this).animate({ marginTop:"-30px"},"slow");
}).mouseout(
    function() {
      $(this).animate({ marginTop:"0px"},"slow");
});
  $('a.rss').mouseover(
     function() {
      $(this).animate({ marginTop:"-30px"},"slow");
}).mouseout(
    function() {
      $(this).animate({ marginTop:"0px"},"slow");
});
  $('li.page').mouseover(
    function() {
    if($(this).children('ul').length > 0) {
      $(this).children('ul').attr('style','opacity:1; margin-left:0px; margin-top:3px; display:block; visibility:visible;');
      $(this).addClass(' sfHover ');
    }
    }).mouseout(
      function() {
        if($(this).children('ul').length > 0) {
          $(this).children('ul').attr('style','opacity:0; margin-left:20px; display:none; visibility:visible;');
          $(this).removeClass(' sfHover ');
        }
});
  $('input.searchFormBlock').focus(function() {
    //alert('dileep');
    this.value="";
    $(this).removeClass(' clearFieldBlurred  ');
    $(this).addClass(' clearFieldActive ');

  }).blur(function() {
    this.value="Search";
    $(this).addClass(' clearFieldBlurred  ');
    $(this).removeClass(' clearFieldActive ');
  });
});
