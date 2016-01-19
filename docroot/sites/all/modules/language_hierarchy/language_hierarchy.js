(function ($) {
  Drupal.behaviors.translationsCollapse = {
    attach: function (context, settings) {
      // Group translations by their parent.
      // This will not produce a hierarchy tree as such, but it's hard to achieve a full hierarchy for incomplete list of languages, which what list of translation is.
      // It's good enough to only group the translations by parent and then render dropdowns with one level depth.
      var languageGroups = {};
      $('li a.language-link', context).each(function () {
        var link = $(this);
        var parent = Drupal.settings.languageHierarchy[link.attr('xml:lang')].parent ? Drupal.settings.languageHierarchy[link.attr('xml:lang')].parent : 'root';
        if (parent) {
          if (!languageGroups[parent]) {
            languageGroups[parent] = [];
          }
          // Inherit active class from parent.
          if (link.parent().hasClass('active')) {
            link.addClass('active');
          }
          languageGroups[parent].push(link);
        }
      });

      // Get descendants of each language.

      // For each, please build a branch

      // Render the list. We expect only two levels, so no need for recursion.
      var picker = jQuery('<ul/>', {
        class: 'language-hierarchy-picker'
      });

      var pickerLabel = jQuery('<label>', {
        text: Drupal.t('Translations') + ':',
        class: 'language-hierarchy-label'
      });

      picker.append(pickerLabel);

      for (var languageGroup in languageGroups) {
        var groupItem = $('<ul/>', {
          class: 'language-hierarchy-group'
        });
        for (var languagePicker in languageGroups[languageGroup]) {
          groupItem.append($('<li/>', {
            class: 'language-hierarchy-item'
          }).append(languageGroups[languageGroup][languagePicker]));
        }

        var languageName = Drupal.settings.languageHierarchy[languageGroup] ? Drupal.settings.languageHierarchy[languageGroup].name : Drupal.t('Global');
        var groupWrapper = $('<li/>', {
          class: 'language-hierarchy-group-item'
        }).append('<p>' + languageName + '</p>');
        groupWrapper.append(groupItem);
        if (groupWrapper.find('.active').length > 0) {
          groupWrapper.addClass('active-trail');
        }
        picker.append(groupWrapper);
      }

      // The list has to slot into the place where old list used to be
      $('.tabs.secondary').replaceWith(picker);
    }
  };
})(jQuery);
