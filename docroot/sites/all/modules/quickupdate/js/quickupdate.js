/**
 * @file
 * Utility functions to handle the searching project buttons.
 */

(function ($) {
  Drupal.behaviors.quickUpdateAutocompleteSearch = {
    attach: function (context, settings) {
      var form = $('#update-manager-update-form');
      $("#edit-add-project", form).click(function(event) {
        var searchProject = $("#edit-search-project").val();
        var otherProjects = $("#edit-other-projects").val();

        // Checks if empty.
        if (searchProject == '') {
          alert(Drupal.t('Please select a project.'));
          return false;
        }
        else {
          var otherProjectsArray = otherProjects.split("\n");
          // Checks if the project has been added.
          if ($.inArray(searchProject, otherProjectsArray) > -1) {
            alert(Drupal.t('The selected project has been added. Please choose another one.'));
            $("#edit-search-project").val('');
            return false;
          }
        }
        if (otherProjects == '') {
          $('#edit-other-projects').val(searchProject);
        }
        else {
          $('#edit-other-projects').val(otherProjects + "\n" + searchProject);
        }
        $("#edit-search-project").val('');
        return false;
      });

      $("#edit-view-project", form).click(function(event) {
        var searchProject = $("#edit-search-project").val();
        window.open("http://www.drupal.org/project/" + searchProject, "quickUpdateViewProject")
        return false;
      });

      $("#edit-clear-project", form).click(function(event) {
        $('#edit-search-project').val('');
        return false;
      });
    }
  };
})(jQuery);
