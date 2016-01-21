jQuery(document).ready(function ($) {
  
  // add the tabs controller to the title
  if ($('h1.main-title').length != 0 && $('#tabs-content').length != 0) {
    $('h1.main-title').append('<span id="tabs-toggle" class="ui-corner-tl ui-corner-tr"><span class="ui-icon ui-icon-plus"></span></span>');
  
  
  // register the click function
  $('#tabs-toggle').click(function() {
    $(this).toggleClass('open').children().toggleClass('ui-icon-plus').toggleClass('ui-icon-minus').parent().parent().toggleClass('noborder');
    $('#tabs-content').slideToggle('slow');
    }); 
  
  // hide the tabs
  $('#tabs-content').hide();
  
  }
  
});