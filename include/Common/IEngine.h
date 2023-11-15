#pragma once
#ifndef __IENGINE__
#define __IENGINE__

class ISessionService;

class IEngine
{
public:
	IEngine(void) {}
	virtual ~IEngine(void) {}

	virtual ISessionService* CreateSessionService(_SessionDesc& desc) = 0;

	//������ �����Ǿ��� ��
	// return value : �ش� Ŭ���̾�Ʈ�� ���� �ø���
	virtual bool OnConnect(int Serial, _SessionDesc& desc) = 0;

	//������ ����Ǿ��� ��
	// Arg1 : ����Ǵ� Ŭ���̾�Ʈ�� ���� �ø���
	virtual bool OnDisconnect(int Serial, _SessionDesc& desc) = 0;

	//Ÿ�̸� �̺�Ʈ�� �߻��� ��
	virtual bool OnTimer(const void* arg) { return false; }

	int GetMaxUserAccept()						{ return maxUserAccept_; }
	void SetMaxUserAccept(int maxUserAccept)	{ maxUserAccept_ = maxUserAccept; }
private:
	int maxUserAccept_ = 5000;
};

#endif // #ifndef __IENGINE__