(function ($) { // JavaScript should be compatible with other libraries than jQuery
  Drupal.behaviors.libanswers = { // D7 "Changed Drupal.behaviors to objects having the methods "attach" and "detach"."
    attach: function(context) {

      // Global.
      var query;

      // Add/remove the default text when they click off/on the input box in the block.
      $("#libanswers-ajax-query").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == 'Don\'t see your question above? Have feedback? Type it here.') {
          this.value = '';
        }
      });
      $("#libanswers-ajax-query").blur(function() {
        if (this.value.length < 1) {
          this.value = 'Don\'t see your question above? Have feedback? Type it here.';
        }
      });

      // On pages where the search box is present, put the descriptive text in the box instead of a heading.
      $("#edit-libanswers-autocomplete").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == 'Search Frequently Asked Questions') {
          this.value = '';
        }
      });
      $("#edit-libanswers-autocomplete").blur(function() {
        if (this.value.length < 1) {
          this.value = 'Search Frequently Asked Questions';
        }
      });

      // Question form labels
      $(".la_askqform_page #pquestion").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == '* Question/Comment') {
          this.value = '';
        }
      });
      $(".la_askqform_page #pquestion").blur(function() {
        if (this.value.length < 1) {
          this.value = '* Question/Comment';
        }
      });
      // --------
      $(".la_askqform_page #pdetails").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == 'More Detail/Explanation') {
          this.value = '';
        }
      });
      $(".la_askqform_page #pdetails").blur(function() {
        if (this.value.length < 1) {
          this.value = 'More Detail/Explanation';
        }
      });
      // --------
      $(".la_askqform_page #pname").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == 'Name') {
          this.value = '';
        }
      });
      $(".la_askqform_page #pname").blur(function() {
        if (this.value.length < 1) {
          this.value = 'Name';
        }
      });
      // --------
      $(".la_askqform_page #pemail").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == '* Email') {
          this.value = '';
        }
      });
      $(".la_askqform_page #pemail").blur(function() {
        if (this.value.length < 1) {
          this.value = '* Email';
        }
      });
      // --------
      $(".la_askqform_page #val4").focus(function() {
        if (this.value.length < 1) {
          this.value = '';
        }
        else if (this.value == 'Library Card Number') {
          this.value = '';
        }
      });
      $(".la_askqform_page #val4").blur(function() {
        if (this.value.length < 1) {
          this.value = 'Library Card Number';
        }
      });

      // Make the form submit submit a live search query.
      $('#libanswers-inner textarea').keypress(function (e) {
        if (e.which == 13) {
          $('#libanswers-ajax-form').submit();
          return false;
        }
      });
      $('#libanswers-ajax-form', context).submit(function() {
        // After the form is submitted, we need to send the contents of the input box as a query to LibAnswers via their Query API.
        query = $('#libanswers-ajax-query', context).val();
        query = encodeURIComponent(query); // html encode it.
        // Get the results from the API call and display them as the new content of the #libanswers-inner div.
        $('#libanswers-inner', context).slideUp(function() {
          $(this).html('<div style="text-align: center; margin-top: 15px;"><img src="/sites/all/modules/contrib/libanswers/images/ajax-loader.gif" alt="Loading" /></div>').slideDown().load('/ask-us/search-results-unthemed?query=' + query, { query : query }, function(data) {
            // Add LibAnswers form when the button is clicked.
            $('#libanswers-inner a#libanswers-please-send', context).click(function() {
              $('#libanswers-inner', context).slideUp(function() {
                $(this).html('<script> var la_iid = \'' + Drupal.settings.libanswers.id + '\'; var la_formelements = [\'intro\', \'question\', \'details\', \'name\', \'email\', \'multi1\', \'multi2\', \'multi3\', \'text1\', \'text2\', \'text3\', \'req\', \'conf\']; var la_css = 0; </script><script src="//api.libanswers.com/js/widget_form.min.js"></script><!--<div id=la_widgetform>--><h2 class=la_header>Ask Us a Question or Leave Feedback</h2><!--</div>--><div id="la_result"></div><form onsubmit="return la_formSubmit(\'Please enter a question.\', \'Question must not be longer than 150 characters.\', \'Invalid email address.\', \'Please answer all required questions.\', \'Error submitting your question. Please reload the page and try again.\', \'Working...\');" method="post" id="la_askqform" name="la_askqform"><input type="hidden" value="' + Drupal.settings.libanswers.id + '" name="instid"><input type="hidden" value="4" name="source"><div class="la_askintro">Have a question? We have answers! Leave your question or comment below and we will reply within 24 hours. Please give an e-mail address so we know where to send your answer. We will not share it.</div><div class="la_field" id="la_question"><label for="pquestion"><span class="reqnote">* </span>Question/Comment:</label> <textarea class="inputtext" id="pquestion" name="pquestion"></textarea></div><div class="la_field" id="la_details"><label for="pdetails">More Detail/Explanation:</label> <textarea class="inputtext" id="pdetails" name="pdetails"></textarea></div><div class="la_field" id="la_name"><label for="pname">Name</label> <input type="text" value="" name="pname" id="pname" class="inputtext"></div><div class="la_field" id="la_email"><label for="pemail"><span class="reqnote">* </span>Email</label> <input type="text" value="" name="pemail" id="pemail" class="inputtext required"></div><div class="la_field" id="la_text1"><label for="val4">Library Card Number</label> <input type="text" value="" name="val4" id="val4" class="inputtext"></div><div id="la_required">Fields marked with <span class="reqnote"> *</span> are required.</div><button id="qsubmitbutton" type="submit" class="fbutton">Submit Your Question</button></form>').slideDown(function() {
                  if (typeof query !== 'undefined' && query != "") {
                    $('textarea#pquestion').val(decodeURIComponent(query)); // Fill in the question with whatever they typed.
                  }
                  $('#la_askqform', context).submit(function() {
                    if ($('#pquestion', context).val() && $('#pemail', context).val()) { // Make sure they entered a question and an email address.
                      libanswers_reset(); // Reset the form back to the beginning
                    }
                    return false;
                  });

                });
              });
              return false;
            });
          });
        });
        return false;
      });

      // Is chat online or offline?
      // We need to show/hide the contents of the block based on chat operator availability. If unavailable, display message.
      if ($('#block-libanswers-libanswers-chat').length > 0) {
        $.ajax({
          url : "http://" + Drupal.settings.libanswers.domain + "/chat_client_status.php",
          dataType : "jsonp",
          data: {
            d: 0,
            iid: Drupal.settings.libanswers.id
          },
          success : function(data) {
            if (data.status) {
              //alert("Its online. Show chat.")
              var s = document.createElement("script");
              s.type = "text/javascript";
              s.src = "http://libanswers.com/js/chat_load_client_in.js";
              $('#block-libanswers-libanswers-chat .content').append(s);
            }
            else {
              //alert("Its offline now. Dont show chat.");
              $('#block-libanswers-libanswers-chat .content').html('Chat is currently offline. Please search our <a href="/ask-us">FAQs</a> or <a href="/ask-us">submit your question here</a> and we will reply shortly.');
            }
          },
          error : function() {
            //alert("Error. Dont show chat.");
            var s = document.createElement("script");
            s.type = "text/javascript";
            s.src = "http://libanswers.com/js/chat_load_client_in.js";
            $('#block-libanswers-libanswers-chat .content').append(s);
          }
        });
      }



    }
  };

  /**
   * Pull in the original HTML of the form if a question gets submitted.
   */
  function libanswers_reset() {
    $('#libanswers-inner').slideUp(function() {
      $(this).html('<div style="text-align: center; margin-top: 15px;"><img src="/sites/all/modules/contrib/libanswers/images/ajax-loader.gif" alt="Loading" /></div>').slideDown().load('/ask-us/block-unthemed', function() {
        Drupal.attachBehaviors('#libanswers-inner');
        // Add a success message.
        $('#libanswers-inner h2').first().after('<div id="la_result" style="display: block;">Thank you! We will contact you when the question is answered.</div>');
      });
    });
  }

})(jQuery);
