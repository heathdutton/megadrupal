/**
 * Change action type.
 */
function form_maker_change_action_type(checked_radio_id) {
  switch (checked_radio_id) {
      case "edit-action-after-submission-radios-stay-on-form":
        document.getElementById("action_after_submission_article").style.display = "none";
        document.getElementById("action_after_submission_custom_text").style.display = "none";
        document.getElementById("action_after_submission_url").style.display = "none";
        // None write in hidden field.
        document.getElementById("form_maker_action_value").value = "stay_on_form";
        break;

      case "edit-action-after-submission-radios-article":
        document.getElementById("action_after_submission_article").style.display = "";
        document.getElementById("action_after_submission_custom_text").style.display = "none";
        document.getElementById("action_after_submission_url").style.display = "none";
        // Article id write in hidden field.
        document.getElementById("form_maker_action_value").value = "article";
        break;

      case "edit-action-after-submission-radios-custom-text":
        document.getElementById("action_after_submission_article").style.display = "none";
        document.getElementById("action_after_submission_custom_text").style.display = "";
        document.getElementById("action_after_submission_url").style.display = "none";
        // Textarea value write in hidden field.
        document.getElementById("form_maker_action_value").value = "custom_text";
        break;

      case "edit-action-after-submission-radios-url":
        document.getElementById("action_after_submission_article").style.display = "none";
        document.getElementById("action_after_submission_custom_text").style.display = "none";
        document.getElementById("action_after_submission_url").style.display = "";
        // Textfield value write in hidden field.
        document.getElementById("form_maker_action_value").value = "url";
        break;
  }
}