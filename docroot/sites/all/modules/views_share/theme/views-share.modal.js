(function ($) {

  Drupal.theme.prototype.viewsShareModal = function () {
    var html = '';

    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div class="ctools-modal-content ctools-modal-views-share">';
    html += '    <div class="popups-container">';
    html += '      <div class="modal-header popups-title clearfix">';
    html += '        <span id="modal-title" class="modal-title"></span>';
    html += '        <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
    html += '      </div>';
    html += '      <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
    html += '    </div>';
    html += '  </div>';
    html += '</div>';

    return html;
  }

})(jQuery);
