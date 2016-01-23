/**
 * Session management tools for CasperJS.
 */

var utils = require('utils');
var cookieName;

/**
 * An object of user session cookies, keyed by unique name, ie, editor, writer.
 */
casper.drupalSessions = {};

/**
 * An array of user objects.
 *
 * Type here an array of all the sessions you would like
 * CasperJS to open when running casper.drupalRun().
 */
casper.drupalUsers = [
  {
    "key": "admin",
    "label": "Administrator",
    "name" : "admin",
    "pass" : "password"
  }
];

/**
 * Sign in a user using a set of credentials.
 *
 * @param Object user
 *   The user object to sign in as.
 */
casper.drupalSignIn = function(user) {
  casper.thenOpen('user', function () {
    this.fill('form#user-login', {
      "name": user.name,
      "pass": user.pass
    }, true);
  });

  casper.waitForSelector('body.logged-in', function() {
    this.log('Logged in as ' + user.label, 'info');
  }, function timeout() {
    this.test.fail('Unable to log in as ' + user.label);
  });
};

/**
 * Creates a session for a user.
 *
 * @param Object user
 *   The user object, with credentials.
 */
casper.drupalCreateSession = function(user) {
  casper.drupalSignIn(user);

  // Store the cookie under a separate name, so we can do the rest of the
  // tests as an anonymous user.
  casper.then(function () {
    casper.each(casper.page.cookies, function (self, cookie) {
      if (cookie.name.match(/^SESS/)) {
        // Store the cookie name so we can use it later.
        cookieName = cookie.name;
        // Store the cookie.
        casper.drupalSessions[user.key] = cookie;
        // Delete the cookie.
        casper.drupalEndSession();
      }
    });
  });

  casper.thenOpen('user', function () {
    var success = !this.exists('body.logged-in') && utils.isObject(casper.drupalSessions[user.key]);
    this.test.assert(success, user.label + ' session cookie has been stored.');
  });
};

/**
 * Begin a session as a user specified by key.
 *
 * @param String key
 *   The key of the user, such as editor, writer, etc.
 */
casper.drupalBeginSession = function(key) {
  casper.then(function() {
    var label = key;
    // If a full user object was passed, parse it for just the key.
    if (!utils.isUndefined(key.key)) {
      label = key.label;
      key = key.key;
    }
    if (utils.isUndefined(casper.drupalSessions[key])) {
      this.test.fail("Unable to find session for user " + key);
    }
    casper.page.addCookie(casper.drupalSessions[key]);
    this.log('Began session as ' + label, 'info');
  });
};

/**
 * End a session, switching back to an anonymous user.
 */
casper.drupalEndSession = function() {
  casper.then(function() {
    casper.page.deleteCookie(cookieName);
    this.log('Ended authenticated session.', 'info');
  });
};

/**
 * Start a test suite, signed in as a certain user.
 *
 * @param String key
 *   The key of the user, such as editor, writer, etc.
 */
casper.drupalStartAs = function(key) {
  casper.start();
  casper.drupalBeginSession(key);
};
