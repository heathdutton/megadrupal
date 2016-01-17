jQuery(document).ready(function($) {
  if($('textarea').length)
  {
    var tag = Drupal.settings.texbar.texbar_tag
    $('textarea.' + tag).markItUp(myLaTeXSettings);
	
    $('.markItUp').after('<div class="texbar-buttons"><input type="button" id="texbar-download-pdf" value="Download"/> <input type="button" id="reset-tex" value="Reset"/></div>');

    $('#reset-tex').click(function() {
      $('textarea.' + tag).val('');
    });

    $('#texbar-download-pdf').click(function() {
      $.ajax({
        type: 'POST',
          url: Drupal.settings.basePath + 'texbar/post',
          dataType: 'json',
          data: { tex: $('textarea.' + tag).val() },
          success: function(data) { window.location = Drupal.settings.basePath + Drupal.settings.texbar.texbar_output + '/output'  + data.output + '.pdf'; },
          error: function() { alert('PDF Generation Failed'); }
      });
    });
  
    $('#clear-latex-textarea').click(function() {
      $('textarea#tex').val('');
    });
  }
}(jQuery));
