function update(income, wealth, tax) {
	tax.value = (0.35 * income.value) + (0.25 * wealth.value);
}