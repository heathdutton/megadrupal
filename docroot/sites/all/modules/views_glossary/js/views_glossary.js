/**
 * For Url mainuplation for itemsperpages and pager, Making itemsperpages and pager independent to expose filter submit button.
 */

(function ($) {
    Drupal.behaviors.views_glossary = {
        attach: function(context, settings) {
            var urlView = window.location.href;
            $('#edit-items-per-page').change(function(){
                var newUrl = urlView.replace(/(&|\?)page=\d+/,'');
                if(newUrl.indexOf('items_per_page') !== -1)
                {
                    window.location.href = newUrl.replace(/items_per_page=\d*/,'items_per_page=' + $(this).val());
                }
                else
                {
                     window.location.href = newUrl + '?items_per_page=' + $(this).val();
                }
            });
            $('#page-selector-select-page-number').change(function(){
                var gotopage = $(this).val() - 1;
                if(urlView.indexOf('&page') !== -1||urlView.indexOf('?page') !== -1)
                {
                    window.location.href = urlView.replace(/(&|\?)page=\d+/,urlView.match(/(\?|&)page=\d+/)[1] + 'page=' + gotopage);
                }

                else
                {
                    if(urlView.indexOf('items_per_page') !== -1){
                    window.location.href = urlView + '&page=' + gotopage;
                    }
                    else
                    {
                        window.location.href = urlView + '?page=' + gotopage;
                    }
                }

            });
        }
    }
}(jQuery));
