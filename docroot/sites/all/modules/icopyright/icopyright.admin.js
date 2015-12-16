// iCopyright admin javascript
(function ($) {

// Update the preview with what the toolbar looks like
function toolbarTouch() {
    var orient = $('#edit-icopyright-article-tools-orientation').val() == 'horizontal' ? 'horz' : 'vert';
    var url = Drupal.settings.icopyright['server'] + '/publisher/TouchToolbar.act?' +
        $.param({
                theme: $('#edit-icopyright-article-tools-theme').val(),
                background: $('#edit-icopyright-article-tools-background').val(),
                orientation: orient,
                publication: Drupal.settings.icopyright['pubid']});
    $('#article-tools-preview').attr('src', url);
    $('#article-tools-preview').attr('height', (orient == 'horz' ? 53 : 130));
    $('#article-tools-preview').attr('width', (orient == 'horz' ? 300 : 100));
    var noticeUrl = Drupal.settings.icopyright['server'] + '/publisher/copyright-preview.jsp?' +
        $.param({
                themeName: $('#edit-icopyright-article-tools-theme').val(),
                background: $('#edit-icopyright-article-tools-background').val(),
                publicationId: Drupal.settings.icopyright['pubid'],
                publicationName: Drupal.settings.icopyright['pubname']});
    $('#copyright-notice-preview').attr('src', noticeUrl);
}

// When the admin form is ready show the preview, and update it whenever the theme changes
$(document).ready(function () {
    toolbarTouch();
    $('#edit-icopyright-article-tools-theme').change(function () {
        toolbarTouch();
    });
    $('#edit-icopyright-article-tools-background').change(function () {
        toolbarTouch();
    });
    $('#edit-icopyright-article-tools-orientation').change(function () {
        toolbarTouch();
    });
});

})(jQuery);