(function ($) {
  Drupal.behaviors.cloudsponge = {
    attach: function(context, settings) {
      function postToDrupal(contacts, source, owner, type) {
        var post_object = {
          type: type,
          emails: [],
          source: source,
          owner: owner,
          csrf_token: settings.cloudsponge.csrf_token
        };

        for (var i = 0; i < contacts.length; i++) {
          post_object['emails'][i] = {
            name: contacts[i].fullName(),
            email: contacts[i].selectedEmail()
          };
        }

        $.post("/cloudsponge/incoming", JSON.stringify(post_object));
      }

      csPageOptions = {
        domain_key: settings.cloudsponge.domain_key,
        initiallySelectedContacts: true,
        beforeDisplayContacts: function(contacts, source, owner) {
          postToDrupal(contacts, source, owner, 'beforeDisplayContacts');

          if (settings.cloudsponge.skip_choosing) {
            if (settings.cloudsponge.redirect && settings.cloudsponge.redirect_url) {
              window.location.replace(settings.cloudsponge.redirect_url);
            }

            return false;
          }
        },
        afterSubmitContacts: function(contacts, source, owner) {
          postToDrupal(contacts, source, owner, 'afterSubmitContacts');

          if (settings.cloudsponge.redirect && settings.cloudsponge.redirect_url) {
            window.location.replace(settings.cloudsponge.redirect_url);
          }
        }
      };
    }
  };
})(jQuery);

