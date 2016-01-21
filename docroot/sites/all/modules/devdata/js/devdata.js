Drupal.behaviors.devdata = {
  attach: function(context) {
    var informations = Drupal.settings.devdata.info;
    var label = Drupal.settings.devdata.infolabel;
    jQuery('body').append('<div id="devdata" class="disabled"><div id="devdata-label">' + label + '</div>\n\
 <div  id="devdata-content" ><div class="inner">' + informations + '</div></div></div>');
    jQuery('#devdata-label').html(jQuery('#devdata-label').text().replace(/(.)/g,"$1<br />"));
    jQuery(".disabled #devdata-label").live("click", function(){
      jQuery('#devdata-content').css('display','block').css('width','270px');
      //fadeIn
      jQuery('#devdata').addClass('enabled');
      jQuery('#devdata').removeClass('disabled');
      jQuery('#devdata').css('width','300px');
    });
    jQuery(".enabled #devdata-label").live("click", function(){
      jQuery('#devdata-content').css('display','none');
      jQuery('#devdata').addClass('disabled');
      jQuery('#devdata').removeClass('enabled');
      jQuery('#devdata').css('width','30px');
    });
  }
};
