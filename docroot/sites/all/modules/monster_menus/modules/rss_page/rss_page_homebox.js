(function ($) {

if (typeof Drupal.homebox_amherst === 'undefined') {
  Drupal.homebox_amherst = {initHooks: {}};
}

Drupal.homebox_amherst.initHooks.initRSSPagePortlet = function(context) {
  if ($(context).is('[class*="homebox-block-rss_page_content"]:not(.rss-page-settings-processed)')) {
    $(context).addClass('rss-page-settings-processed');
    $('.portlet-settings', context)
      .unbind('click')
      .click(function() {
        var portlet = $(this).closest('.homebox-portlet');
        var url = Drupal.settings.basePath + '?q=rss_page_block_form/' + Drupal.homebox_amherst.activeTab.rel + '/' + $('[name=rss-page-block-key]', context).val();
        Drupal.homebox_amherst.pleaseWait();
        $.ajax({
          url:      url,
          cache:    'false',
          dataType: 'html',
          success:  function(data) {
            Drupal.homebox_amherst.pleaseWait(false);
            if (!data) return;

            var buttons = {};
            buttons[Drupal.t('Save')] = function() {
              var form_data = {};
              $('#rss-page-block-form :input').each(function() {
                var elem = $(this), name = elem.attr('name');
                if (name && (elem[0].checked || elem[0].type != 'checkbox' && elem[0].type != 'radio')) {
                  form_data[name] = elem.val();
                }
              });
              var dialog = this;
              var errmsg = Drupal.t('Your changes could not be saved. Please reload this page and try again.');
              Drupal.homebox_amherst.pleaseWait($(this));
              $.ajax({
                url:      url,
                cache:    'false',
                type:     'POST',
                data:     form_data,
                dataType: 'json',
                success:  function(result) {
                  if (!result) result = {error: errmsg};
                  if (result.error) {
                    Drupal.homebox_amherst.showError(result.error);
                    result.html = '';
                  }
                  $(dialog).dialog('close');
                  if (result.html) {
                    $(portlet).slideUp(function() {
                      Drupal.attachBehaviors($(result.html)
                        .hide()
                        .replaceAll(this)
                        .slideDown()
                        .parent()
                      );
                    });
                  }
                },
                error:    function() {
                  Drupal.homebox_amherst.showError(errmsg);
                  $(dialog).dialog('close');
                }
              });
            };
            buttons[Drupal.t('Cancel')] = function() {
              $(this).dialog('close');
            };

            var dialog = $('<div />');
            $('#rss-page-block-form').remove();
            Drupal.attachBehaviors($(data).appendTo('body'));
            $('#rss-page-block-form')
              .appendTo(dialog)
              .dialog({
                zIndex:   90,
                modal:    true,
                width:    450,
                title:    Drupal.t('RSS feeds'),
                buttons:  buttons,
                close:    function() {
                  $(this).dialog('destroy').remove();
                }
              });
          },
          error:    function() {
            Drupal.homebox_amherst.showError(Drupal.t('The widget could not be edited. Please reload this page and try again.'));
            Drupal.homebox_amherst.pleaseWait(false);
          }
        });
      });
  }
};

})(jQuery);
