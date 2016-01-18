Ranks the content using the IMDB voting algorithm.

Provides the "imdb" aggregate function to the Voting API module. Use the name of
this function instead of average or count when creating Views for modules based
on Voting API.

IMDB-like votes are calculated using the formula W = (Rv + Cm) / (v + m) where
W = IMDB rating, R = mean for the movie as a number from 0 to 10 (mean), v =
number of votes for the movie, m = minimum votes required to be listed in the
Top 250, C = the mean vote across the whole report (6.9 by default).
