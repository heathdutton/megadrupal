Trick Question version 1.3 for Drupal 7.x
-----------------------------------------

A very small and simple CAPTCHA-type spam prevention module to use as an
alternative to larger and much more complex and facility rich modules such as
the excellent and very popular Captcha module and the sub module Captcha
Riddler or the intelligent and flexible Mollom module.

CAPTCHA is short for Completely Automated Public Turing-test to tell Computers
and Humans Apart, and that is exactly what all these modules do. Perform a
test to separate fake submissions from real ones.

Independent
This module is a stand alone module, independent of Captcha and other modules.
It contains only one simple questions (riddle) and nothing else. It's not
dependent on any other modules, libraries or functions, and does not use
graphical elements.
It's flexible and can be used on most forms: node forms as well as user
registrations, comment forms and other forms.
It offers you to select one or several forms and to select roles that don't
need to be checked, in other words won't see and need to reply to the
question.

The idea is to have a really simple question with a very obvious answer. For
localized sites you can ask and request replies in your own language for even
better bot submission prevention and to almost completely avoid fake
submissions from non-local humans, who won't be able to read the questions.
You will never be able to completely avoid fake submissions from real humans,
though.

Examples
It's recommended to use questions that can be replied to with one letter in
order to make the workload on the user as small as possible, but other simple
questions and replies will work equally well.
Examples:
"What's the sum of three and four as a digit?" The correct answer is "7"
"What's the first letter in the word salmon?" The correct answer is "s" or
"S"
"Tell us Mary Poppins' first name" Correct answer is "mary", "Mary", "MARY" or
any combination.
Don't use the above examples, but make your own with similar questions and
replies. Since you supply both the question and the correct answer, you are
responsible for making it logical and easy to understand.

The module ignores letter case in the reply, and "mary", "Mary", "MARY" and
"mARy" will all be correct replies to the last example.

Caveat
Please notice that the Trick Question field is NOT marked as a required field
(usually a red asterisk), but that it IS required to be correctly filled out
for the form to validate.
This construction is used because Drupal will require you to fill out the
trick question to delete a node, which is pretty counter intuitive. This is a
core Drupal issue and not an issue for this module only. Any required field
need to be filled if it's empty in order for Drupal to accept a deletion.

Configuration
You configure the module on its configuration page in admin > settings >
trick_question
Type in a question and the correct answer.
Edit the explanation to tell the users what is happening, and maybe even
reveal the correct reply. Since the bots won't be able to read this tip,
there's no harm done in giving away the correct answer.
There is no functions to automatically change the question and answer and no
function to randomly pick from a list. You yourself will have to go in and
change the question and answer now and then if you want a higher level of
security.

Now select the forms you want to filter.
You can select any node form as well as the user registration form and the
comment submission form. Should you have a form that doesn't appear in the
list, simply add the Drupal form ID in the text field below the checkboxes.
Save the settings and your question should appear on the relevant forms.

Permissions
Admin is never asked to fill out the Trick Question, so if you want to test
this function, you need to be logged in as a user with a role that has to
reply.
You might want to go to the permissions page and exclude other user groups
(roles) from the test. If you have groups of already tested and trusted
people, you may have no need to pester them with the question again. Mark them
in the permissions list.
You can at the same time allow certain roles to administer the module and
change the question. This privilege should of course only be granted to site
administrators, editors, webmasters and similar groups, and never to common
users, no matter how trusted they might be.

