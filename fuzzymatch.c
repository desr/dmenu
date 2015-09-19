int
compare_distance(const void *a, const void *b) {
	Item const *da = *(Item **) a;
	Item const *db = *(Item **) b;
	if(!db)
		return 1;
	if(!da)
		return -1;
	return da->distance - db->distance;
}

void
fuzzymatch(void) {
	/* bang - we have so much memory */
	Item *item;
	Item **fuzzymatches = NULL;
	char c;
	int number_of_matches = 0, i, pidx, sidx, eidx;
	int text_len = strlen(text), itext_len;

	matches = matchend = NULL;

	/* suppress compiler warning for unused match function */
	if(0)
		match();

	/* walk through all items */
	for(item = items; item && item->text; item++) {
		if(text_len) {
			itext_len = strlen(item->text);
			pidx = 0;
			sidx = eidx = -1;
			/* walk through item text */
			for(i = 0; i < itext_len && (c = item->text[i]); i++) {
				/* fuzzy match pattern */
				if(text[pidx] == c) {
					if(sidx == -1)
						sidx = i;
					pidx++;
					if(pidx == text_len) {
						eidx = i;
						break;
					}
				}
			}
			/* build list of matches */
			if(eidx != -1) {
				/* compute distance */
				/* factor in 30% of sidx and distance between eidx and total
				 * text length .. let's see how it works */
				item->distance = eidx - sidx + (itext_len - eidx + sidx) / 3;
				appenditem(item, &matches, &matchend);
				number_of_matches++;
			}
		}
		else
			appenditem(item, &matches, &matchend);
	}

	if(number_of_matches) {
		/* initialize array with matches */
		if(!(fuzzymatches = realloc(fuzzymatches, number_of_matches * sizeof(Item*))))
			printf("cannot realloc %u bytes:", number_of_matches * sizeof(Item*));
		for(i = 0, item = matches; item && i < number_of_matches; i++, item = item->right) {
			fuzzymatches[i] = item;
		}
		/* sort matches according to distance */
		qsort(fuzzymatches, number_of_matches, sizeof(Item*), compare_distance);
		/* rebuild list of matches */
		matches = matchend = NULL;
		for(i = 0, item = fuzzymatches[i];  i < number_of_matches && item && \
				item->text; i++, item = fuzzymatches[i]) {
			appenditem(item, &matches, &matchend);
		}
		free(fuzzymatches);
	}
	curr = sel = matches;
	calcoffsets();
}
