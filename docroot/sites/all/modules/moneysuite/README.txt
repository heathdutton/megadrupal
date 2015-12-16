We have compiled here all modules, one module per directory.
Gateways pack have been integrated as normal gateways in Core. That includes CCBill, Fortumo, Stripe, Credits, CoinPayments and Payal REST API.
You must choose which module to activate depending on the functionalities you require. It is not mandatory to have all modules activated.
The only module you are probably going to require is Core.
We will include more modules as users provide us them. That is, we are missing File Downloads, Bonus Pack, Events Ticketing Suite,
Pay to Publish and Marketplace Suite.

Suggested upgrade path (should be tested by the community):

We have tested a method for substituting moneyscripts modules with moneysuite modules. We would like though that the community tests it
in their test installs to make sure nothing is broken.

To upgrade follow the steps below:

    Deactivate moneyscripts modules
    With FTP remove MoneyScripts files from directory
    With FTP copy MoneySuite files into directory
    Activate MoneySuite modules
    Everything should be working ok, otherwise generate an issue in the queue
