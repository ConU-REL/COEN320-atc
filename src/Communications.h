#pragma once

class Communications {
public:
	static Communications& getInstance();

	Communications();
	virtual ~Communications();
};
