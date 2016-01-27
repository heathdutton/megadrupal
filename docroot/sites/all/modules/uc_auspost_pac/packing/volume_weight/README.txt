Packing Method: Volume and Weight
=================================

A packing algorithm that uses both volume (optional) and weight limits to
separate cart products into a parcel configuration which is then used to quote
on shipping them.

When using volume limits, this method is not particularly accurate when
converting volume back into dimensions which is what the AusPost PAC
Domestic Parcel service requires. The dimensions are derived only from the
volume and girth limitations, which means they are the optimal dimensions for
that volume and not based on what it would pack into in reality.

In some circumstances, the optimal dimensions and what happens in reality
result in the same quoted amount. Your should test to see if your products pack
efficiently with this algorithm prior to using it.

Your site may wish to include a markup on the shipping price to compensate for
under/over charging, or use a custom module to hook into the processing to
alter it in your own personal way. See the Packing API and this module's API
for details.

As always, you must test to see if this module works for you, and if it is
appropriate.

eg. In the most extreme possible case, a cart with 2 items in it. 1x1x105cm
and 34x35x35cm rigid objects, weighing 20kg and being sent from Brisbane to
Sydney. A human might pack these into a box 35x35x105cm, the maximum possible
size with a volume of 128625 cm3. The computer only knows the volume as 41755
cm3, figuring the dimensions for the optimal package to be 35x35x35cm. The
human packed one is quoted at $42.30, but the computer packed one is $29.95.

eg. 2x 10x35x35 + 3x35x105 = 35525. 20kg total weight. This algorithm packs
this as 33x33x33 for a total of $29.95. A human would pack it to 23x35x105
(84525cm3) which would cost $31.85.
