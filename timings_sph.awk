# Awk script to get time profile of multistep run
# This version is for "consph" runs
# Run this script with:
# awk -f timings_sph.awk < DIAG
# where DIAG is the diagnostic output from ChaNGa
#
BEGIN { rung = 0;
    maxrung = 30;
    for (tmp = 0; tmp < maxrung; tmp++) {
	counts[tmp] = 0;
	gravity[tmp] = 0.0;
	dd[tmp] = 0.0;
	loadb[tmp] = 0.0;
	build[tmp] = 0.0;
	udot[tmp] = 0.0;
    }
	tgrav = tudot = tdd = tloadb = tbuild = 0.0;
	rung = 0;
	tsf = 0.0;
	tfb = 0.0;
}
/Gravity Active/ {
    rung = $6
    }
/^total / { dd[rung] = dd[rung] + $2; }
/^Domain decomposition ... total/ { dd[rung] = dd[rung] + $5; }
/^took / { loadb[rung] = loadb[rung] + $2; }
/Building trees / { build[rung] = build[rung] + $5; }
/Calculating gravity and SPH / { counts[rung]++ ; gravity[rung] = gravity[rung] + $6; }
/^uDot/ { udot[rung] += $7 ; }
/^Star Formation/ { tsf += $5 ; }
/^Distribute/ { tfb += $9 ; }
END {
    while(counts[maxrung-1] == 0) {
	maxrung--;	 
	}
    steps = counts[maxrung-1];
    
    print "Rung, counts, Gravity/SPH, uDot, DomainD, LoadB and TBuild";
    for (tmp = 0; tmp < maxrung; tmp++) {
	print tmp, counts[tmp], gravity[tmp], udot[tmp], dd[tmp], loadb[tmp], build[tmp];
	tgrav = tgrav + gravity[tmp];
 	tudot += udot[tmp];
	tdd = tdd + dd[tmp];
	tloadb = tloadb + loadb[tmp];
	tbuild = tbuild + build[tmp];
    }
    print "Totals\nGrav, Star Form, FeedBack, udot, DomainD, LoadB and TBuild";
    print  tgrav, tsf, tfb, tudot, tdd, tloadb, tbuild;
    }
