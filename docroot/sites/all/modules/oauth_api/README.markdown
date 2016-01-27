## OAuth consumer.

OAuth should be easy; we shouldn’t have to write any code for authentication mechanism and token storage. OAuth API was designed with a few simple goals in mind:

* Providing an API to programmer instead of a full end-user module.
* Low footprint, lightweight, fast
* Using PECL native OAuth extension
* Convention over configuration (provided with helper methods)
* Adaptable (You can write your own class Adapter to extends another OAuth library)
* High cohesion with Drupal framework.
* Maintainable: code base is very small and based on Adapter Design Pattern. This ensures to be fast to port to any new Drupal version.
* Dependable: Fully tested and errors are logged in the watchdog for you.
 
With drupal 7 class registry, you don’t need to import any files to instantiate your OAuth objet, simply create it with your module name as a parameter and you’re done.

    $oauth = new OAuthAdapter(“twitter”);
    $oauth->setCaller($user->uid);
    $oauth->fetch("url");
