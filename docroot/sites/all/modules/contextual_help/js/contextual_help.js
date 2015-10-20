
/**
* Provide the HTML to create the modal dialog.
*/
(function ($) {
Drupal.theme.prototype.contextual_help_modal = function () {
  var html = '';
  html += '<div id="ctools-modal" class="popups-box">';
  html += '  <div class="ctools-modal-content ctools-modal-contextual-help-modal-content">';
  html += '    <div class="modal-header ctools-modal-contextual-help-modal-content">';
  html += '      <span id="modal-title" class="modal-title"></span>';
  html += '      <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeImage + '</a></span>';
  html += '    </div>';
  html += '    <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
  html += '  </div>';
  html += '</div>';
  return html;
}
})(jQuery);
