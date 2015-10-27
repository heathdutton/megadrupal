(function ($) {
  Drupal.behaviors.ajaxdocumentviewer = {
    attach: function (documentviewer) {
      $('.show_iframe').click(function () {
        var fid = $(this).attr('fid');
        var divId = "#document-viewer-" + fid;
        $('.ajaxdocumentview-iframe').hide();
        $(divId).show();
        return false;
    });
 }
};
})(jQuery);
