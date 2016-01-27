(function($){
  var img = new Image();
  
  var refreshThrobber = function() {
    var src = Drupal.settings.basePath + 
      Drupal.settings.CToolsReasonsBounceModal.throbberSrc;
    img.src = src;
  }
  
  Drupal.behaviors.reasons_bounce_close = {
    attach: function() {      
      refreshThrobber();
      
      window.onbeforeunload = function(e) {
        e = e || window.event;
        var text = Drupal.settings.reasons_bounce.message;
        if (e) {
          e.returnValue = text;
        }
        setTimeout(response_bounce_popup_form(), 1000);
        return text;
      }

      $('a, form, select, div').each(function(index, value){
        $(this).click(function(e) {
          window.onbeforeunload = function(e) {};
        });
      })
    }
  }
  
  function response_bounce_popup_form() {
    Drupal.CTools.Modal.show('CToolsReasonsBounceModal');
    $('.ctools-modal-content .modal-throbber-wrapper').html(img);
    $.ajax({
      type: "POST",
      url: Drupal.settings.basePath + 'reasons_bounce/ajax/' + Drupal.settings.reasons_bounce.template,
      success: function (data) {
        $('#modal-title').html(data[1].title);
        $('#modal-content').html(data[1].output);
        $('a, form, select, div').each(function(index, value){
          $(this).click(function(e) {
            window.onbeforeunload = function(e) {};
          });
        })
      },
      dataType: 'json'
    });
  }
  
  /**
* Provide the HTML to create the modal dialog.
*/
Drupal.theme.prototype.CToolsReasonsBounceModal = function () {
  var html = '';

  html += '<div id="ctools-modal" class="popups-box">';
  html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
  html += '    <table cellpadding="0" cellspacing="0" id="ctools-face-table">';
  html += '      <tr>';
  html += '        <td class="popups-tl popups-border"></td>';
  html += '        <td class="popups-t popups-border"></td>';
  html += '        <td class="popups-tr popups-border"></td>';
  html += '      </tr>';
  html += '      <tr>';
  html += '        <td class="popups-cl popups-border"></td>';
  html += '        <td class="popups-c" valign="top">';
  html += '          <div class="popups-container">';
  html += '            <div class="modal-header popups-title">';
  html += '              <span id="modal-title" class="modal-title"></span>';
  html += '              <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
  html += '              <div class="clear-block"></div>';
  html += '            </div>';
  html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
  html += '            <div class="popups-buttons"></div>'; //Maybe someday add the option for some specific buttons.
  html += '            <div class="popups-footer"></div>'; //Maybe someday add some footer.
  html += '          </div>';
  html += '        </td>';
  html += '        <td class="popups-cr popups-border"></td>';
  html += '      </tr>';
  html += '      <tr>';
  html += '        <td class="popups-bl popups-border"></td>';
  html += '        <td class="popups-b popups-border"></td>';
  html += '        <td class="popups-br popups-border"></td>';
  html += '      </tr>';
  html += '    </table>';
  html += '  </div>';
  html += '</div>';

  return html;
}
  
})(jQuery);  