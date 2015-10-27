
Commerce Pricelist module
-------------------------

Set prices on products from several pricelists. This is common in B2B.

Pricelists can be assigned per role and per user.
Price list rows can apply based on quantity or order date.

So for example user 0 can have one price for product A if ordering less
than 10 and another price above 10, and this discount only applies until
some date. User role wholesale customer can have other prices than
anonymous users and so on.

Each user / role etc can have many pricelists connected and a pricelist
may contain only a few prices, in which case the module will continue to
search next pricelist for a missing price. This means that a pricelist
may be used to contain (time limited) discounts that are only valid for
some customers on some quantities.

A hook is provided for anyone who wants to apply other criteria than user
and role, such as setting price lists on user edit forms etc.

Feeds integration with an example excel/csv importer is included. Feeds
importers can be attached to price lists, which is done by giving the
price lists a faux node id and using feeds "attach to node form" function.
Hopefully feeds move away from nids in the future so this "hack"
can be avoided.

Pricelists and priccelist items are both implemented as entities and
leverages rules to set prices.
