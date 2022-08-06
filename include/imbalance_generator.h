#ifndef IMBALANCE_GENERATOR_H
#define IMBALANCE_GENERATOR_H

double ewma(double prev, double current, double alpha) {
	return (1 - alpha) * prev + alpha * current;
}

class ImbalanceGenerator {
	double exp_num_trades_;
	double exp_buy_vol_ratio_;

public:
	ImbalanceGenerator() {
	}
};

#endif
