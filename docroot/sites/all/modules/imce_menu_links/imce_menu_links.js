/*
 * Add the IMCE link to the Path field.
 */
(function ($) {
  var imce_menu_links = window.imce_menu_links = {queues: {}};
  
  //Drupal behavior that process file fields.
  Drupal.behaviors.imce_menu_links = {attach: function(context, settings) {
    var set = settings.imce_menu_links;  
    var $button = $('#imce-menu-link-submit', context);
    
    var $wrapper = $(document.createElement('span')).addClass('imce-menu-links-wrapper');  
    var $opener = $(document.createElement('a')).addClass('imce-menu-links-opener').attr({href: '#'});
    $opener.text(imce_menu_links.openerText()).click(function() {
      window.open(set.url + '/' + '?app=imce-menu-links|sendto@imce_menu_links.imcesendto', '', 'width=760,height=560,resizable=1');
      return false;
    });
    $wrapper.insertAfter('#edit-link-path').append($opener).append($button);
  }};
  
  // Handle what is return by IMCE.
  imce_menu_links.imcesendto = function(file,win) {
    $('#edit-link-path').val('<file>' + decodeURIComponent(file.url));
    win.close();
  }
  
  // Submits a field widget with a file id.
  imce_menu_links.submit = function(fieldID, fid) {
    $('#' + fieldID + '-imce-filefield-fid').val(fid);
    $('#' + fieldID + '-imce-filefield-submit').mousedown();
  };
  
  // Returns text for the opener link.
  imce_menu_links.openerText = function() {
    return Drupal.t('Open File Browser');
  };

})(jQuery);