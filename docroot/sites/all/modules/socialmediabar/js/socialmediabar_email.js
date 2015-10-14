/**
 * @file
 * Handles the ShareThis e-mail sharing setup.
 */

stLight.options({
  publisher: Drupal.settings.socialmediabar.apikey,
  doNotHash: false,
  doNotCopy: false,
  hashAddressBar: false
});
