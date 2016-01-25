
(function ($) {
  var state = $("#edit-state-autocomplete").val() + "/"
  if ($("#edit-country").val()) {
    $("#edit-state-autocomplete").val(state + $("#edit-country").val());
    Drupal.autocompleteAutoAttach();
  }
  $("#edit-country").change(function() {
    $("#edit-state-autocomplete").val(state + $(this).val());
    Drupal.autocompleteAutoAttach();
  })
});