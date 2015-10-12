
Geofield Proximity-to-Polygons for Views
========================================

An add-on for the Geofield module to calculate distances to polygons and
multi-polygons in Views on single-valued and multi-valued Geofields.
It employs the following rule:
  if the origin/reference point is inside the polygon or one of the polygons
  that are part of the multi-polygon or multi-valued field, then zero is
  returned; otherwise the result is the distance to the closest vertex (corner
  point)

Automatically converts Geofield proximity fields, filters and sorts to exhibit
the above behaviour.
No configuration, just select the Geofield proximity field, filter or sort in
you View as per normal and the module does the rest.
The module deals correctly with Views that contain a mix of polygons and other
geometries, like single points. Distance calculation for points will continue to
take place on the database. The module will not affect those results.

Restrictions
------------
When you request the Geofield-proximity field in the Views UI, you must also
add the normal Geofield field and drag it in position before the Geofield-
proximity field. You may exclude the Geofield from display using the tick box.
Because polygon proximities are calculated as part of the post-execution of the
FIELD, you must include the field in your View when using filter or sort. If
you don't want to see these proximities in the output, tick the Exclude box.

Tip
---
When you use multi-valued Geofields on a MAP (e.g. Leaflet) under the Multiple
Field Settings of the Views UI configuration panel for the field, tick the box
"Display all values in the same row". When you use mutli-valued Geofields in a
TABLE and you wish to see distances for each polygon that is part of the
geofield, UNTICK that same box.


Suggestions for extensions / roadmap
------------------------------------
o extend module to include distance to linestrings and multi-linestrings
o improve algorithm to return distance to closest vertex OR edge
o remove dependency on Leaflet function to parse Geofield results in Views
o point-in-polygon calculation is based on a flat coordinate system of lat/lon,
  but distance calculation takes curvature of the earth into account;
  address discrepancies that may occur for very wide polygons

Rik de Boer, Melbourne Nov 2014

The point-in-polygon implementation in this module was adapted from the C-code in
http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html#Originality
Copyright (c) 1970-2003, Wm. Randolph Franklin

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
