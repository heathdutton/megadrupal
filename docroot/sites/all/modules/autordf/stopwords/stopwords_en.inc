<?php

// English Stopwords
function autordf_stopwords_en() {
  $sp = 'able, about, above, abst, accordance, according, accordingly, across,' .
    ' act, actually, added, adj, adopted, affected, affecting, affects, after,' .
    ' afterwards, again, against, all, almost, alone, along, already, also,' .
    ' although, always, among, amongst, and, announce, another, any, anybody,' .
    ' anyhow, anymore, anyone, anything, anyway, anyways, anywhere, apparently,' .
    ' approximately, are, aren, arent, arise, around, aside, ask, asking, auth,' .
    ' available, away, awfully, back, became, because, become, becomes, becoming,' .
    ' been, before, beforehand, begin, beginning, beginnings, begins, behind,' .
    ' being, believe, below, beside, besides, between, beyond, biol, both, brief,' .
    ' briefly, but, came, can, cannot, can, cause, causes, certain, certainly,' .
    ' com, come, comes, contain, containing, contains, could, couldnt, date, did,' .
    ' didn, different, does, doesn, doing, done, don, down, downwards, due,' .
    ' during, each, edu, effect, eight, eighty, either, else, elsewhere, end,' .
    ' ending, enough, especially, etl, etc, even, ever, every, everybody,' .
    ' everyone, everything, everywhere, except, far, few, fifth, first, five,' .
    ' fix, followed, following, follows, for, former, formerly, forth, found,' .
    ' four, from, further, furthermore, gave, get, gets, getting, give, given,' .
    ' gives, giving, goes, gone, got, gotten, had, happens, hardly, has, hasn,' .
    ' have, haven, having, hed, hence, her, here, hereafter, hereby, herein,' .
    ' heres, hereupon, hers, herself, hes, hid, him, himself, his, hither,' .
    ' home, how, howbeit, however, hundred, immediate, immediately, importance,' .
    ' important, inc, indeed, index, information, instead, into, invention,' .
    ' inward, isn, itd, itl, its, itself, just, keep, keeps, kept, keys, know,' .
    ' known, knows, largely, last, lately, later, latter, latterly, least, less,' .
    ' lest, let, lets, like, liked, likely, line, little, look, looking, looks,' .
    ' ltd, made, mainly, make, makes, many, may, maybe, mean, means, meantime,' .
    ' meanwhile, merely, might, million, miss, more, moreover, most, mostly, mrs,' .
    ' much, mug, must, myself, name, namely, nay, near, nearly, necessarily,' .
    ' necessary, need, needs, neither, never, nevertheless, new, next, nine,' .
    ' ninety, nobody, non, none, nonetheless, noone, nor, normally, nos, not,' .
    ' noted, nothing, now, nowhere, obtain, obtained, obviously, off, often,' .
    ' okay, old, omitted, once, one, ones, only, onto, ord, other, others,' .
    ' otherwise, ought, our, ours, ourselves, out, outside, over, overall,' .
    ' owing, own, page, pages, part, particular, particularly, past, per,' .
    ' perhaps, placed, please, plus, poorly, possible, possibly, potentially,' .
    ' predominantly, present, previously, primarily, probably, promptly, proud,' .
    ' provides, put, que, quickly, quite, ran, rather, readily, really, recent,' .
    ' recently, ref, refs, regarding, regardless, regards, related, relatively,' .
    ' research, respectively, resulted, resulting, results, right, run, said,' .
    ' same, saw, say, saying, says, sec, section, see, seeing, seem, seemed,' .
    ' seeming, seems, seen, self, selves, sent, seven, several, shall, she,' .
    ' shed, shel, shes, should, shouldn, show, showed, shown, showns, shows,' .
    ' significant, significantly, similar, similarly, since, six, slightly,' .
    ' some, somebody, somehow, someone, somethan, something, sometime,' .
    ' sometimes, somewhat, somewhere, soon, sorry, specifically, specified,' .
    ' specify, specifying, state, states, still, stop, strongly, sub,' .
    ' substantially, successfully, such, sufficiently, suggest, sup, sure,' .
    ' take, taken, taking, tell, tends, than, thank, thanks, thanx, that,' .
    ' thatl, thats, thate, the, their, theirs, them, themselves, then, thence,' .
    ' there, thereafter, thereby, thered, therefore, therein, therel, thereof,' .
    ' therere, theres, thereto, thereupon, theree, these, they, theyd, theyl,' .
    ' theyre, theye, think, this, those, thou, though, thoughh, thousand,' .
    ' throug, through, throughout, thru, thus, til, tip, together, too, took,' .
    ' toward, towards, tried, tries, truly, try, trying, twice, two, under,' .
    ' unfortunately, unless, unlike, unlikely, until, unto, upon, ups, use,' .
    ' used, useful, usefully, usefulness, uses, using, usually, value, various,' .
    ' very, via, viz, vol, vols, want, wants, was, wasn, way, wed, welcome, wel,' .
    ' went, were, weren, wee, what, whatever, whatl, whats, when, whence,' .
    ' whenever, where, whereafter, whereas, whereby, wherein, wheres, whereupon,' .
    ' wherever, whether, which, while, whim, whither, who, whod, whoever, whole,' .
    ' whol, whom, whomever, whos, whose, why, widely, willing, wish, with,' .
    ' within, without, won, words, world, would, wouldn, www, yes, yet, you,' .
    ' youd, youl, your, youre, yours, yourself, yourselves, youe, zero';
    
  $list = explode(', ', $sp);
  return array('ar' => $list);
}