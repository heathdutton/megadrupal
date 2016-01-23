// Add an event listener for messages posted to this window
window.addEventListener('message', receiveMessage, false);

// Define the message handler function
function receiveMessage(event) {

  // Make sure the message posted to this window is from Coinbase
  if (event.origin == 'https://coinbase.com') {
    var event_type = event.data.split('|')[0];     // "coinbase_payment_complete"
    var event_id   = event.data.split('|')[1];     // ID for this payment type
    var complete = Drupal.settings.coinbaseCompleteCheckout;

    if (event_type == 'coinbase_payment_complete') {
      window.location = complete;
    }
  }
}
