package org.usfirst.frc.team846.robot;
 
public class TimeStats {
	double min, max, avg, sq;
	long count, logcount, initcount;
	double prev;
	public TimeStats(int l, int i) {
		min = 1E6; max = 0; avg = 0; count = 0; sq = 0;
		prev = 0;
		logcount = l; initcount = i;
	}
	public void Record (double timer) {
		double duration = timer - prev;
		if (count > initcount) {
			if (duration > max) max = duration;
			if (duration < min) min = duration;
			double avgNew = (avg*(count-initcount) + duration) / (1.0 + count - initcount);
			sq += (duration - avg) * (duration - avgNew);
			avg = avgNew;

			if ((count % logcount) == 0) {
				System.out.println("Max " + max + "; Min " + min + "; Avg " + 
						avg + "; Count " + count + "; stDev " + Math.sqrt(sq / (count - initcount)));
			}
		}
		prev = timer;
		count ++;
	}
}
