#include "MyApp.h"
#include "MainFrame.h"

#include "IsoChecking.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	wxInitAllImageHandlers();

	m_mainFrame = new MainFrame(nullptr, -1, "Beyond The Forbidden Lands - Launcher");
	m_mainFrame->Maximize();
	m_mainFrame->Show();

	m_mainFrame->GetOverviewPanel()->GetPatchNotes()->SetContent("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse scelerisque tellus sed nibh varius egestas. Morbi id dictum justo. In sed elementum est. Quisque tristique, enim nec dictum dictum, eros augue porta lorem, sit amet iaculis augue velit a quam. Curabitur id nisl eget orci tristique fermentum nec in nisi. Curabitur rhoncus justo ac metus porta, eget dapibus mi volutpat. Integer sollicitudin scelerisque augue a accumsan. Cras fermentum ipsum et dignissim placerat. Aenean at mollis velit. Curabitur convallis lorem a accumsan consectetur. Donec et sodales erat. Nam non augue ipsum. Aenean efficitur ornare mauris at consequat. Proin tempor sollicitudin ultricies."
		"\n\nInteger in lorem rutrum, fermentum augue at, iaculis tellus.Pellentesque hendrerit in nulla vel dapibus.Sed non augue elementum, blandit velit sed, scelerisque elit.Praesent aliquet odio mauris, in rhoncus nulla vehicula id.Fusce sollicitudin, felis vel aliquet vestibulum, nibh nunc placerat nibh, vitae accumsan velit augue eu nulla.Nam in venenatis quam.Donec lectus nunc, venenatis id odio eu, consectetur imperdiet felis.Morbi ut dapibus libero, in viverra felis.Donec in pharetra nibh."
		"\n\nMaecenas malesuada arcu vitae nibh hendrerit, nec ultricies enim porttitor.Aenean et mattis felis, nec maximus arcu.In sit amet ligula venenatis, congue sem et, tempor urna.Curabitur eget luctus lorem.Ut malesuada erat sed eros tincidunt luctus.Suspendisse placerat consectetur libero at auctor.Nam quis hendrerit ligula.Suspendisse potenti.Mauris semper sapien ac arcu scelerisque blandit non ut tellus.Aenean quis metus dignissim, tristique elit nec, commodo augue.Suspendisse rutrum nibh eu varius sodales.Nullam tincidunt turpis ante, vel convallis ante pellentesque a.Nulla vitae tortor feugiat, malesuada turpis vitae, posuere nulla.Cras ac nulla vestibulum, dapibus tellus vel, ornare mauris.Integer aliquam nisi finibus, maximus massa at, consequat ex.Nulla in massa ut ligula pulvinar dapibus et in nibh."
		"\n\nEtiam gravida turpis a massa pretium dignissim.Duis euismod malesuada felis, vitae tincidunt lectus pellentesque vitae.Curabitur at nisl porttitor nisl ullamcorper ultrices.Quisque dictum est et ex aliquam pellentesque a id ante.Suspendisse blandit nulla sit amet elit sollicitudin, eget gravida leo vestibulum.Sed a ipsum vel odio commodo volutpat.Ut pretium nisl molestie ex egestas tincidunt.Vestibulum ac nisi vel tortor laoreet rhoncus sed quis sapien.Ut vulputate tortor ac neque auctor, et sollicitudin neque fringilla.Vivamus aliquet est sed blandit tincidunt.Donec quis cursus est.Vestibulum vestibulum venenatis egestas."
		"\n\nAenean cursus, risus sed tincidunt sagittis, ipsum nisl bibendum mauris, eget luctus justo nisi eget nisl.Sed fringilla quis neque nec semper.Etiam nec massa ex.Nam nibh metus, posuere quis varius vitae, eleifend quis erat.Aliquam purus neque, ullamcorper sed dictum non, malesuada venenatis sapien.Sed dapibus aliquet arcu ut ornare.Sed nibh ex, fermentum sit amet facilisis vitae, porta vulputate nisl.Cras in turpis eu ante volutpat tempor.Nunc varius odio justo, id faucibus ipsum placerat eget.Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus."
		"\n\nSuspendisse in tempus eros, ut elementum nisi.Vestibulum laoreet porttitor feugiat.Morbi dapibus sem vel suscipit mattis.Sed eleifend dolor quis nulla.");

	m_mainFrame->GetOverviewPanel()->GetPatchNotes()->SetTitle("Patch Notes");
	return true;
}