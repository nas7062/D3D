#pragma once
class Billboard : public Actor
{
public:
	static Billboard* Create(string name = "Billboard");
	virtual void	Update() override;
	virtual void	Render() override;
};

