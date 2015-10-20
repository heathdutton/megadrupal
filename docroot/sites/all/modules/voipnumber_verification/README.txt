== Introduction ==

The voipnumber_verification.module is a module which adds verification method
for VoIP Number Fields (https://drupal.org/project/voipnumberfield)
by sending SMS message with randomly generated 4-digit code. In future versions
we plan to add verification via Phone call too.

In particular, this module provides:

 - a new VoIP Number Field option, which can be found under
 Field Settings -> VoIP Number Verification


== Installation ==

1. Extract voipnumber_verification.module to your sites/all/modules directory

2. Enable the VoIP Number Verification module in admin/build/modules

3. Make sure you have configured VoIP Drupal (https://drupal.org/project/voipdrupal)
with at least one VoIP Provider in order to be able to send SMS messages with
verification code.

== Using VoIP Number Verification ==

1. Go to Field Settings of existing VoIP Number Field or create new
   VoIP Number Field

2. Check "Must be verified" checkbox in order to require Number verification
   for this field.
   Optionally change Verification text.

3. Create new content. Now you will see "Verify number via SMS" button next to
  VoIP Number Field. Enter the Phone Number and then press the button. You
  should receive SMS with 4-digit code. Use it to verify number in next step.

4. When you clicked button in step 3, you should see textfield where you should
   enter the verification code. Enter the code you received in SMS and click
   submit. If the code is correct you should see "Your number has been verified"
   message.

5. Save the content. Entered number will be saved in database as "verified".

---
The VoIP Number Verification module has been originally developed by Tamer Zoubi
under the sponsorship of the MIT Center for Future Civic Media (http://civic.mit.edu).

