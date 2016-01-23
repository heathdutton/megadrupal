/**
 * Kills sessions for all users.
 */
casper.test.begin("Logging out sessions for all users.", function suite(test) {
  casper.start();

  casper.then(function() {
    casper.each(casper.drupalUsers, function(self, user) {
      casper.drupalBeginSession(user);
      casper.thenOpen('user/logout', function() {
        test.assertHttpStatus(200, user.label + ' has successfully logged out.');
      });
    });
  });

  casper.run(function() {
    test.done();
  });

});
