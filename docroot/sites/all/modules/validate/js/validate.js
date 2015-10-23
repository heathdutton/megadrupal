$(document).ready(function() {
    var validate = Drupal.settings.validate;
    if (validate) {
        $.each(validate, function(key, value) {
            var container_name =  "#" + key.replace(/_/g, '-');
            
            value.errorContainer = container_name + "-error";
            value.errorLabelContainer = container_name + "-error ul";
            value.wrapper = "li";
            
            $(container_name).validate(value);
        });
    }
});
