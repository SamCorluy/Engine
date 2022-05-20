#pragma once
class BaseCommand
{
public:
	BaseCommand() = default;
	virtual ~BaseCommand() = default;

	BaseCommand(const BaseCommand& other) = delete;
	BaseCommand(BaseCommand&& other) = delete;
	BaseCommand& operator=(const BaseCommand& other) = delete;
	BaseCommand& operator=(BaseCommand&& other) = delete;

	virtual void Execute() = 0;
};

