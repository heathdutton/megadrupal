Commerce shipping weight tariff
===============================

Allows the creation of complex matrices of shipping tariffs by order weight,
across multiple shipping services.
For example:
             < .5kg | < 2kg | < 10kg
 Normal    |   £2   |  £4   |  £6
 Express   |   £4   |  £6   |  £8

Each tariff is stored as a product entity with:
 - a weight field for the maximum weight for the tariff
 - a shipping service field to indicate the service this belongs to
 - the usual Commerce price fields, to give the price of the tariff.
These products should not be referenced by nodes, and will not be added to
the cart; rather, their price is returned to Commerce Shipping as the
shipping service rate.

If you find this module useful, please consider helping advance its development
with any of the above.
